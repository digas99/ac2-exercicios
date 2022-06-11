	.equ get_char, 2
	.equ put_char, 3
	
	.data
	.text
	.globl main
main:					# do {
	li $v0, get_char	# 	c = getChar()
	syscall				#
	move $a0, $v0		#
	addiu $a0, $a0, 1	#
	li $v0, put_char	#	putChar(c+1)
	syscall				#
	bne $a0, '\n', main	# while (c != '\n')
	li $v0, 0			# return 0
	jr $ra
	