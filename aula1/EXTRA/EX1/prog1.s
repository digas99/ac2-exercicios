# MAPA DE REGISTOS
# state:	$s0
# cnt:		$s1
	
	.equ UP, 1
	.equ DOWN, 0
	
	.equ putChar, 2
	.equ printInt, 7
	.equ inkey, 1
	
	.data
	.text
	.globl main
main:
    addiu $sp, $sp, -16
	sw $ra, 0($sp)
	sw $s0, 4($sp)				#state
	sw $s1, 8($sp)				#cnt
	sw $s2, 12($sp)				#c

	li $s0, 0					# state = 0
	li $s1, 0					# cnt = 0
do:								# do {
	li $v0, putChar				# 	putChat('\r')
	li $a0, '\r'				#
	syscall						#	
	li $v0, printInt			#	printInt(cnt, 10 | 3 << 16)
	move $a0, $s1				#
	li $a1, 3					#
	sll $a1, $a1, 16			#
	or $a1, $a1, 10				#
	syscall						#
	li $v0, putChar				# 	putChar('\t')
	li $a0, '\t'				#
	syscall						#
	li $v0, printInt			# 	printInt(cnt, 2 | 8 << 16)
	move $a0, $s1				#
	li $a1, 8					#
	sll $a1, $a1, 16			#
	or $a1, $a1, 2				#
	syscall						#
	li $a0, 5					#	wait(5)
	jal wait					#
	li $v0, inkey				#	c = inkey()
	syscall 					#
	move $s2,$v0				#
if1:							#	if( c == '+' )
    bne $s2, '+', if2			#		state = UP
    li $s0, UP					#
if2:							#	if( c == '-' )
    bne $s2, '-', if3			#		state = DOWN
    li $s0, DOWN				#
if3:							#	if( state == UP ) {
    bne $s2, UP, else			#		cnt = (cnt + 1) & 0xFF
    addi $s1, $s1, 1			#
    andi $s1, $s1, 0xFF   		#
    j endif						#	}
else:							#	else
    addi $s1, $s1, -1			#		cnt = (cnt - 1) & 0xFF
    andi $s1, $s1, 0xFF			#
endif:							#	}
    bne $s2, 'q', do			# } while( c != 'q' )
    li $v0, 0					# return 0
    
	lw $ra, 0($sp)
	lw $s0, 4($sp)				#state
	lw $s1, 8($sp)				#cnt
	lw $s2, 12($sp)				#c
	addiu $sp, $sp, 16
	
    jr $ra
    
wait:
	li $t0, 0
for:							# for(i=0; i < 515000 * ts; i++
	mul $t1, $a0, 515000
	bge $t0, $t1, endfor
	addi $t0, $t0, 1
endfor:
    jr $ra
	
	
	
	
	
	
	
	
	
