	.equ PRINT_STR, 8
	.equ READ_CORE_TIMER, 11
	.equ RESET_CORE_TIMER, 12
	.data
s:	.asciiz "hello"
	.text
	.globl main
main:	addiu	$sp, $sp, -4
	sw 	$ra, 0($sp)
while:	li	$a0, 500	# ms = 100
	jal 	delay		# delay(100)
	la	$a0, s		# printStr("hello")
	li 	$v0, PRINT_STR	#
	syscall			#
	j 	while		# while (1)
	lw	$ra, 0($sp)
	addiu	$sp, $sp, 4
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
