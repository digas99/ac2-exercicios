# Mapa de registos
# SFR_BASE_HI:	$s0
# ms:			$s1
# value:		$s2
# flag:			$s3
	
	.equ SFR_BASE_HI, 0xBF88
	.equ TRISE, 0x6100
	.equ PORTE, 0x6110
	.equ LATE, 0x6120
	.equ readCoreTimer, 11
	.equ resetCoreTimer, 12
	.equ K, 20000
	.equ printInt, 6
	
	.data
	.text
	.globl main
main:
	addiu $sp, $sp, -20		# preserve values in stack
	sw $ra, 0($sp)			#
	sw $s0, 4($sp)			#
	sw $s1, 8($sp)			#
	sw $s2, 12($sp)			#
	sw $s3, 16($sp)			#
		
	la $s0, SFR_BASE_HI		# $s0 = SFR_BASE_HI
	lw $t0, TRISE($s0)		# $t0 = TRISE
	and $t0, $t0, 0xFFE1	# $t0 = TRISE & 0xFFE1 // RE4-1 as outputs
	sw $t0, TRISE($s0)		# // update value of TRISE
	li $s1, 1000			# ms = 1/7*1000 = 1000/7
	div $s1, $s1, 7			#
	li $s3, 0				# flag = 0;
while:						# while(1) {
	lw $s2, PORTE($s0)		# $t0 = PORTE
	and $s2, $s2, 0xFFE1	# $t0 = PORTE & 0xFFE1
	bnez $s3, else			# $t0 = (PORTE & 0xFFE1) | (!flag ? 0x0012 : 0x000C)
	or $s2, $s2, 0x0012		#
	j endif					#
else:						#
	or $s2, $s2, 0x000C		#
endif:						#
	sw $s2, LATE($s0)		# LATE = $t0
	xor $s3, $s3, 1			# flag = !flag

	li $v0, printInt		# 	printInt(value, 2 | 4 << 16)
	move $a0, $s2			#
	li $a1, 4				#
	sll $a1, $a1, 16		#
	ori $a1, $a1, 2			#
	syscall					#
	
	move $a0, $s1			#	delay(ms)
	jal delay				#
	j while					# }
endwhile:
	
	lw $ra, 0($sp)			# retreive values from stack
	lw $s0, 4($sp)			#
	lw $s1, 8($sp)			#
	lw $s2, 12($sp)			#
	lw $s3, 16($sp)			#
	addiu $sp, $sp, 20		#
	
	jr $ra
	
# delay(ms)
delay:							# delay(ms) {
	li $v0, resetCoreTimer		# 	resetCoreTimer()
	syscall						#
while_d:						#	while(readCoreTimer < K * ms);
	mul $t0, K, $a0				# 	// $t0 = K*ms;
	li $v0, readCoreTimer		#	// $v0 = readCoreTimer()
	syscall						#
	bge $v0, $t0, endwhile_d	#
	j while_d					# }
endwhile_d:						#
	jr $ra