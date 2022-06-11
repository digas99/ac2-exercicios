	.equ SIZE, 20
	.equ printStr, 8
	.equ readStr, 9
	.equ printInt, 6
	.equ printInt10, 7
	
	.data
s1:
	.asciiz "Introduza 2 strings: "
s2:
	.asciiz "Resultados:\n"
str1:
	.space 21 # SIZE + 1
str2:
	.space 21 # SIZE + 1
str3:
	.space 41 # 2 * SIZE + 1
	.text
	.globl main
main:
	addiu $sp, $sp, -4			# preserve values in stack
	sw $ra, 0($sp)				#
	
	li $v0, printStr			# printStr(s1)
	la $a0, s1					#
	syscall						#
	li $v0, readStr				# readStr(str1, SIZE)
	la $a0, str1				#
	li $a1, SIZE				#
	syscall						#
	li $v0, readStr				# readStr(str2, SIZE)
	la $a0, str2				#
	li $a1, SIZE				#
	syscall						#
	li $v0, printStr			# printStr(s2)
	la $a0, s2					#
	syscall						#	
	la $a0, str1				# // $a0 = strlen(str1)
	jal strlen					#
	move $a0, $v0				#
	li $v0, printInt			# printInt(strlen(str1), 10)
	li $a1, 10					#
	syscall						#
	la $a0, str2				# // $a0 = strlen(str2)
	jal strlen					#
	move $a0, $v0				#
	li $v0, printInt			# printInt(strlen(str2), 10)
	li $a1, 10					#
	syscall						#	
	la $a0, str3				# strcpy(str3, str1)
	la $a1, str1				#
	jal strcpy					#
	la $a0, str3				# // $a0 = strcat(str3, str1)
	la $a1, str1				#
	jal strcat					#
	li $v0, printStr			# printStr(strcat(str3, str1))
	syscall						#
	la $a0, str1				# // $a0 = strcmp(str1, str2)
	la $a1, str2				#
	jal strcmp					#
	li $v0, printInt10			# printInt10(strcmp(str1, str2))
	syscall						#	
	li $v0, 0					# return 0
	
	lw $ra, 0($sp)				# retreive values from stack
	addiu $sp, $sp, 4			#
	
	jr $ra
	
	
# int strlen(char *str)
strlen:
	li $v0, 0					# // len = 0
for_strlen:						# for( len = 0; *str != '\0'; len++, str++ )
	lb $t1, 0($a0)				# // $t1 = *str
	beq $t1, 0, endfor_strlen	#
	addi $v0, $t0, 1			# // len++
	addiu $a0, $a0, 1			# // str++
	j for_strlen				#
endfor_strlen:					# return len (len already being saved on $v0)
	jr $ra
	
# char *strcpy(char *dst, char *src)
strcpy:
	move $v0, $a0				# *p = dst
for_strcpy:						# for( ; ( *dst = *src ) != '\0'; dst++, src++ )
	lb $t1, 0($a1)				# // $t1 = *src
	sb $t1, 0($a0)				# // *dst = *src
	beq $t1, 0, endfor_strcpy	#
	addiu $a0, $a0, 1			# // dst++
	addiu $a1, $a1, 1			# // src++
	j for_strcpy				#
endfor_strcpy:					# return p (p already saved on $v0)
	jr $ra
	
# char *strcat(char *dst, char *src)
strcat:
	addiu $sp, $sp, -8			# preserve values in stack
	sw $ra, 0($sp)				#
	sw $s0, 4($sp)				#

	move $s0, $a0				# *p = dst
for_strcat:						# for( ; *dst != '\0'; dst++ )
	lb $t0, 0($a0)				# // $t0 = *dst
	beq $t0, 0, endfor_strcat	#
	addiu $a0, $a0, 1			# // dst++
	j for_strcat				#
endfor_strcat:					#
	jal strcpy					# strcpy(dst, src) // $a0 and $a1 already defined
	move $v0, $s0				# return p
	
	lw $ra, 0($sp)				# retreive values from stack
	lw $s0, 4($sp)				#
	addiu $sp, $sp, 8			#
	
	jr $ra
	
# int strcmp(char *str1, char *str2)
strcmp:
for_strcmp:	# useless, just for visual purposes
								# for( ; (*str1 == *str2) && (*str1 != '\0'); str1++, str2++ )
	lb $t0, 0($a0)				# // $t0 = *str1
	lb $t1, 0($a1)				# // $t1 = *str2
	bne $t0, $t1, infor_strcmp	#
	beq $t0, 0, endfor_strcmp	#
infor_strcmp:					#
	addiu $a0, $a0, 1			# // str1++
	addiu $a1, $a1, 1			# // str2++
	j for_strcmp # or j strcmp
endfor_strcmp:					#
	sub $v0, $t0, $t1			# return (*str1 - *str2)
	jr $ra