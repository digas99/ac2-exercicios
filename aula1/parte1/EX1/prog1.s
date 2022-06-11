	.equ print_str, 8
	
	.data
msg:
	.asciiz "AC2 - Aulas práticas\n"
	.text
	.globl main
main:
	la $a0, msg
	li $v0, print_str
	syscall				# print_str(msg)
	li $v0, 0			# return 0
	jr $ra