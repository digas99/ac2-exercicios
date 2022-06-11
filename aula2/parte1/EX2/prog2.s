	.equ READ_CORE_TIMER, 11
	.equ RESET_CORE_TIMER, 12
	.equ PUT_CHAR, 3
	.equ PRINT_INT, 6
	.equ K, 20000
	
	.data
	.text
	.globl main
main:
	addiu $sp, $sp, -8			# preserve values in stack
	sw $ra, 0($sp)				#
	sw $s0, 4($sp)				#

	li $s0, 0					# counter = 0
while1:							# while(1) {
	li $a0, 100					#	delay(10)
	jal delay					#
	li $v0, PRINT_INT			#
	move $a0, $s0				#	printInt(counter++, 10 | 4 << 16)
	li $a1, 4					#
	sll $a1, $a1, 16			#
	or $a1, $a1, 10				#
	syscall
	addi $s0, $s0, 1			#
	li $v0, PUT_CHAR			#	putChar('\r');
	li $a0, '\r'				#
	syscall						#
	j while1					# }
	li $v0, 0					# return 0
	
	lw $ra, 0($sp)				# retreive values from stack
	lw $s0, 4($sp)				#
	addiu $sp, $sp, 8			# 

	jr $ra
	
	
delay:
	li $v0, RESET_CORE_TIMER		# resetCoreTimer()
	syscall							#
	mul $t0, $a0, K					# // $t0 = k * ms
delay_while:						# while(readCoreTimer() < K * ms)
	li $v0, READ_CORE_TIMER			#
	syscall							#
	blt $v0, $t0, delay_while		#
	jr $ra