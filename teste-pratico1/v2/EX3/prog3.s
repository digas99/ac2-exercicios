# Mapa de registos
# $s0:	SFR_BASE_HI
# $s1:	value
# $s2:	ms	
	
	.equ SFR_BASE_HI, 0xBF88
	.equ TRISE, 0x6100
	.equ PORTE, 0x6110
	.equ LATE, 0x6120
	.equ K, 20000
	.equ resetCoreTimer, 12
	.equ readCoreTimer, 11
	.equ printInt, 6
	
	.data
freq:
	.float 3.5
	.text
	.globl main
main:
	addiu $sp, $sp, -16			# preserve values in stack
	sw $ra, 0($sp)				#
	sw $s0, 4($sp)				#
	sw $s1, 8($sp)				#
	sw $s2, 12($sp)				#
	
	li $s0, SFR_BASE_HI			# $s0 = SFR_BASE_HI
	lw $t0, TRISE($s0)			# $t0 = TRISE
	# X000 00XX
	and $t0, $t0, 0xFF83		# set RE6-2 as outputs
	
	li $s1, 0					# value = 0
	li $t0, 1000				# ms = 1000/3.5
	mtc1 $t0, $f4				# $f4 = 1000
	la $t1, freq				# $f6 = freq
	l.s $f6, 0($t1)				#
	div.s $f4, $f4, $f6			#
	cvt.w.s $f4, $f4			#
	mfc1 $s2, $f4				# $s2 = ms
while:							# while(1) {
	lw $t0, LATE($s0)			#	$t0 = LATE
	and $t0, $t0, 0xFF83		#	// preserve all bits except RE6-2
	sll $t1, $s1, 2				#	// shift value because bits start in RE2
	or $t0, $t0, $t1			#	LATE = (LATE & 0xFF83) | (value << 2)
	sw $t0, LATE($s0)			#	// update LATE
	li $v0, printInt			# 	printInt(value, 2 | 5 << 16)
	move $a0, $s1				#
	li $a1, 5					#	$a1 = 5
	sll $a1, $a1, 16			#	$a1 = 5 << 16
	ori $a1, $a1, 2				#	$a1 = 2 | 5 << 16
	syscall						#
	bnez $s1, else1				#	if (value == 0)
	li $s1, 25					#		value = 25
	j endif1					#
else1:							#	else
	addi $s1, $s1, -1			#		value--
endif1:							#
	move $a0, $s1				#
	jal delay					#	delay(ms)
	j while						# }
	
	lw $ra, 0($sp)				# retreive values from stack
	lw $s0, 4($sp)				#
	lw $s1, 8($sp)				#
	lw $s2, 12($sp)				#
	addiu $sp, $sp, 16			#
	
	jr $ra
	
#####################
# delay(ms)
delay:							# void delay(int ms) {
	li $v0, resetCoreTimer		# 	resetCoreTimer()
	syscall						#
while_delay:					#	while(readCoreTimer() < K * ms);
	mul $a0, $a0, K				#	// $a0 = ms * K
	li $v0, readCoreTimer		#	// $v0 = readCoreTimer()
	syscall						#
	blt $v0, $a0, while_delay	#
	jr $ra