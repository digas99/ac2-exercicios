	.equ PUT_CHAR, 3
	.equ PRINT_INT, 6
	.equ READ_CORE_TIMER, 11
	.equ RESET_CORE_TIMER, 12
	.data
	.text
	.globl main
main:	addu 	$sp, $sp, -4
        sw 	$ra, 0($sp)

while:	lui 	$t1, 0xBF88
	lw 	$t2, 0x6050($t1)
	andi	$t2, $t2, 0x0000000F
	move	$a0, $t2
	li	$a1, 0x00040002		# para a impressão em binário com 4 bits, o valor a colocar
					# no registo $a1 é 0x00040002
	li	$v0, PRINT_INT
	syscall
	
	li	$a0, '\n'		# NEW LINE
	li 	$v0, PUT_CHAR  	 	#
	syscall				#
	
	li	$t1, 1000
	addi	$t2, $t2, 1		# valSwitches++
	div	$a0, $t1, $t2		# $a0 = KT_ms = 1000/(valSwitches+1)
	jal	delay
	
	j 	while
	
	addu 	$sp, $sp, 4
        lw 	$ra, 0($sp)

	jr	$ra

######
delay:	move 	$t0, $a0		# $t0 = ms
for:	subu 	$t0, $t0, 1		# ms--
	li 	$v0, RESET_CORE_TIMER	# resetCoreTimer()
	syscall				#
while2:	li 	$v0, READ_CORE_TIMER	# readCoreTimer()
	syscall				#
	blt 	$v0, 20000, while2	# if (readCoreTimer() < 20000) go to while
	bgtz 	$t0, for		# if (ms > 0) go to for
	jr 	$ra