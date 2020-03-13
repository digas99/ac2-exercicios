	.equ	PRINT_STR, 8
	
	.data
msg:	.asciiz	"AC2 - DETPIC32\n"
	.text
	.globl 	main
main:	la	$a0, msg
	li	$v0, PRINT_STR	# printStr("AC2 - DETPIC32\n");
	syscall	
	li	$v0, 0		# return 0;
	jr	$ra