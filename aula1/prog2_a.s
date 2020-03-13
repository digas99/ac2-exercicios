	.equ	GET_CHAR, 2
	.equ	PUT_CHAR, 3
	
	.data
	.text
	.globl main
main:					
do:	li	$v0, GET_CHAR		# c = getChar();
	syscall				#
	move	$t1, $v0		# if (c != '\n')
	beq	$t1, '\n', while	# 
	
	# addi	$t1, $t1, 1	---->	This adds 1 to the ASCII code of the char, displaying the next char ('a' -> 'b')
		
	move	$a0, $t1		# 	putChar(c);
	li	$v0, PUT_CHAR		# 
	syscall				#
while:	bne	$t1, '\n', do		# while (c != '\n') jump to do
	li	$v0, 0			# return 0;
	jr	$ra
