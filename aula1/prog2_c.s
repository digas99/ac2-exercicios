	.equ	READ_INT10, 5
	.equ	PRINT_INT, 6
	.equ	PRINT_STR, 8
	.equ	PRINT_INT10, 7
	
	.data
msg1:	.asciiz	"\nIntroduza um número (sinal e módulo): ";
msg2:	.asciiz	"\nValor lido, em base 2: ";
msg3:	.asciiz	"\nValor lido, em base 16: ";
msg4:	.asciiz	"\nValor lido, em base 10 (unsigned): ";
msg5:	.asciiz	"\nValor lido, em base 10 (signed): ";
enter:	.asciiz	"\n";
	.text
	.globl main
main:	la	$a0, msg1	# printStr("\nIntr...");
	li	$v0, PRINT_STR	#
	syscall			#
	li	$v0, READ_INT10	# readInt10();
	syscall
	move	$t1, $v0	# value = readInt10();
	
	la	$a0, msg2	# printStr("\nValor...");
	li	$v0, PRINT_STR	#
	syscall			#
	move	$a0, $t1	# printInt(value, ...);
	li	$a1, 2		# printInt(..., 2);
	li	$v0, PRINT_INT	# printInt(value, 2);
	syscall			#
	
	la	$a0, msg3	# printStr("\nValor...");
	li	$v0, PRINT_STR	#
	syscall			#
	move	$a0, $t1	# printInt(value, ...);
	li	$a1, 16		# printInt(..., 16);
	li	$v0, PRINT_INT	# printInt(value, 16);
	syscall			#
	
	la	$a0, msg4	# printStr("\nValor...");
	li	$v0, PRINT_STR	#
	syscall			#
	move	$a0, $t1	# printInt(value, ...);
	li	$a1, 10		# printInt(..., 10);
	li	$v0, PRINT_INT	# printInt(value, 10);
	syscall			#
	
	la	$a0, msg5	# printStr("\nValor...");
	li	$v0, PRINT_STR	#
	syscall			#
	move	$a0, $t1	# printInt10(value);
	li	$v0, PRINT_INT10# 
	syscall			#
	
	la	$a0, enter	# printStr("\n");
	li	$v0, PRINT_STR	#
	syscall			#
	
	j	main		# while (1);
	
	li	$v0, 0		# return 0;
	jr	$ra
