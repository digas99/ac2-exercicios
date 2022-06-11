# Mapa de registos
# cnt1:		$s0	
# cnt5:		$s1	
# cnt10:	$s2	
# frq1:		$s3	
# frq5:		$s4	
# frq10:	$s5
# mult:		$s6
# c:		$v0

	.equ K, 20000
	.equ putChar,3
	.equ printInt, 6
	.equ printInt10, 7
	.equ resetCoreTimer, 12
	.equ readCoreTimer, 11
	.equ inkey, 1
	
	.data
	.text
	.globl main
main:
	addiu $sp, $sp, -32		# preserve values in stack
	sw $ra, 0($sp)			#
	sw $s0, 4($sp)			#
	sw $s1, 8($sp)			#
	sw $s2, 12($sp)			#
	sw $s3, 16($sp)			#
	sw $s4, 20($sp)			#
	sw $s5, 24($sp)			#
	sw $s6, 28($sp)			#
	
	li $s0, 0				# cnt1 = 0
	li $s1, 0				# cnt5 = 0
	li $s2, 0				# cnt10 = 0
	li $s3, 10				# frq1 = 10
	li $s4, 5				# frq5 = 5
	li $s5, 1				# frq10 = 1;
	li $s6, 1				# mult = 1;
while:						# while(1) {
	li $v0, inkey			#	c = inkey()
	syscall					#
	beq $v0, 'A', if1		#	if (c == 'A' || c == 'a') {
	bne $v0, 'a', endif1	#	
if1:						#
	mul $s3, $s3, 2			#		frq1*=2
	mul $s4, $s4, 2			#		frq5*=2
	mul $s5, $s5, 2			#		frq10*=2
	mul $s6, $s6, 2			#		mult*=2
endif1:						#	}

	beq $v0, 'N', if2		#	if (c == 'N' || c == 'n') {
	bne $v0, 'n', endif2	#	
if2:						#
	li $s3, 10				# 		frq1 = 10
	li $s4, 5				# 		frq5 = 5
	li $s5, 1				# 		frq10 = 1;
	li $s6, 1				# 		mult = 1;	
endif2:						#	}
	addi $s0, $s0, 1		#	cnt1++
	div $t1, $s3, $s4		#	// $t0 = frq1 / frq5
	rem $t0, $s0, $t1		#	// $t0 = cnt1%(frq1/frq5)
	bne $t0, 0, endif3		#	if (cnt1%(frq1/frq5) == 0) {
	addi $s1, $s1, 1		#		cnt5++
endif3:						#	}
	div $t1, $s3, $s5		#	// $t0 = frq1 / frq10
	rem $t0, $s0, $t1		#	// $t0 = cnt1%(frq1/frq10)
	bne $t0, 0, endif4		#	if (cnt1%(frq1/frq10) == 0) {
	addi $s2, $s2, 1		#		cnt10++
endif4:						#	}
	li $v0, putChar			#	putChar('\r')
	li $a0, '\r'			#
	syscall					#
	li $v0, putChar			#	putChar('x')
	li $a0, 'x'				#
	syscall					#
	li $v0, printInt10		#	printInt10(mult)
	move $a0, $s6			#
	syscall					#
	li $v0, putChar			#	putChar(' ')
	li $a0, ' '				#
	syscall					#
	li $v0, printInt		#	printInt(cnt1, 10 | 5 << 16)
	move $a0, $s0			#
	li $t0, 5				#	// $t0 = 10 | 5 << 16
	sll $t0, $t0, 16		#
	ori $t0, $t0, 10		#
	move $a1, $t0			#
	syscall					#
	li $v0, putChar			#	putChar(' ')
	li $a0, ' '				#
	syscall					#
	li $v0, printInt		#	printInt(cnt5, 10 | 5 << 16)
	move $a0, $s1			#
	move $a1, $t0			#
	syscall					#
	li $v0, putChar			#	putChar(' ')
	li $a0, ' '				#
	syscall					#
	li $v0, printInt		#	printInt(cnt10, 10 | 5 << 16)
	move $a0, $s2			#
	move $a1, $t0			#
	syscall					#
	li $a0, 1000			#	delay(1000/frq1)
	div $a0, $a0, $s3		#
	jal delay				#
	j while					# }
	li $v0, 0				# return 0

	lw $ra, 0($sp)			# retreive values from stack
	lw $s0, 4($sp)			#
	lw $s1, 8($sp)			#
	lw $s2, 12($sp)			#
	lw $s3, 16($sp)			#
	lw $s4, 20($sp)			#
	lw $s5, 24($sp)			#
	lw $s6, 28($sp)			#
	addiu $sp, $sp, 32		# 
	jr $ra
	
delay:
	li $v0, resetCoreTimer		# resetCoreTimer()
	syscall						#
while_delay:					# while(readCoreTimer() < K * ms)
	li $t0, K					#
	mul $t0, $t0, $a0			#	// $t0 = k * ms
	li $v0, readCoreTimer		#
	syscall						#
	blt $v0, $t0, while_delay	#
	jr $ra
