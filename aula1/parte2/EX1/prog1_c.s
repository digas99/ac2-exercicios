	.equ print_str, 8
	.equ print_int, 6
	.equ print_int10, 7
	.equ read_int10, 5
	
	.data
str1:
	.asciiz "\nIntroduza um inteiro (sinal e módulo): "
str2:
	.asciiz "\nValor em base 10 (signed): "
str3:
	.asciiz "\nValor em base 2: "
str4:
	.asciiz "\nValor em base 2: "
str5:
	.asciiz "\nValor em base 10 (unsigned): "
str6:
	.asciiz "\nValor em base 10 (unsigned), formatado: "		
	.text
	.globl main
main:						# while(1) {
	li $v0, print_str		#	printStr(str1)
	la $a0, str1			#
	syscall					#
	li $v0, read_int10		#	value = readInt10()
	syscall					#
	move $a0, $v0			#
	li $v0, print_str		#	printStr(str2)
	la $a0, str2			#
	syscall					#
	li $v0, print_int10		#	print_int10(value)
	syscall					#
	li $v0, print_str		#	printStr(str3)
	la $a0, str3			#
	syscall					#
	li $v0, print_int		#	print_int(value, 2)
	li $a1, 2				#
	syscall					#
	li $v0, print_str		#	printStr(str4)
	la $a0, str4			#
	syscall					#
	li $v0, print_int		#	print_int(value, 16)
	li $a1, 16				#
	syscall					#
	li $v0, print_str		#	printStr(str5)
	la $a0, str5			#
	syscall					#
	li $v0, print_int		#	print_int(value, 10)
	li $a1, 10				#
	syscall					#
	li $v0, print_str		#	printStr(str6)
	la $a0, str6			#
	syscall					#
	li $v0, print_int		#	print_int(value, 10 | 5 << 16)
	li $a1, 10				#
	ori $a1, $a1, 5			#
	sll $a1, $a1, 16		#
	syscall					#
	j main					# }
	li $v0, 0				# return 0
	jr $ra

	