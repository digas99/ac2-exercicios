	.equ	INKEY, 1
	.equ	PUT_CHAR, 3
	
	.data
	.text
	.globl main
main:					
do:	li	$v0, INKEY		# c = inkey();
	syscall				#
	move	$t1, $v0		#
	beq	$t1, 0, do		# while(c == 0) jump to do
	beq	$t1, '\n', while	# if (c != '\n')
	move	$a0, $t1		# 	putChar(c);
	li	$v0, PUT_CHAR		# 
	syscall				#
while:	bne	$t1, '\n', do		# while (c != '\n') jump to do
	li	$v0, 0			# return 0;
	jr	$ra
