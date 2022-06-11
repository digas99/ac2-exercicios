# a) Qual a frequência de incremento da variável counter?
# 200kHz, ou seja 10 ms
	
	.equ READ_CORE_TIMER, 11
	.equ RESET_CORE_TIMER, 12
	.equ PUT_CHAR, 3
	.equ PRINT_INT, 6
	
	.data
	.text
	.globl main
main:
	li $t0, 0					# counter = 0
while1:							# while(1) {
	li $v0, RESET_CORE_TIMER	# 	resetCoreTimer()
	syscall						#
while2:							#	while(readCoreTimer() < 200000)
	li $v0, READ_CORE_TIMER		#
	syscall						#
	blt $v0, 200000, while2		#
	li $v0, PRINT_INT			#
	move $a0, $t0				#	printInt(counter++, 10 | 4 << 16)
	li $a1, 4					#
	sll $a1, $a1, 16			#
	or $a1, $a1, 10				#
	syscall
	addi $t0, $t0, 1			#
	li $v0, PUT_CHAR			#	putChar('\r');
	li $a0, '\r'				#
	syscall						#
	j while1					# }
	li $v0, 0					# return 0
	jr $ra