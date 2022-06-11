	.equ inkey, 1
	.equ put_char, 3
	
	.data
	.text
	.globl main
main:					# do {
	li $v0, inkey		# 	c = inkey()
	syscall				#
	beq $v0, 0, else	#	if (c != 0) {
	move $a0, $v0		#
	li $v0, put_char	#		putChar(c)
	syscall				#
	j endif				#	}
else:					#	else {
	li $a0, '.'			#
	li $v0, put_char	#		putChar('.')
	syscall				#
endif:					#	}
	bne $a0, '\n', main	# while (c != '\n')
	li $v0, 0			# return 0
	jr $ra
	