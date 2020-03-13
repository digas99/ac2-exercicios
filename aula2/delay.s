	.equ READ_CORE_TIMER, 11
	.equ RESET_CORE_TIMER, 12
	.data
	.text
	.globl delay
delay:	move 	$t0, $a0		# $t0 = ms
for:	subu 	$t0, $t0, 1		# ms--
	li 	$v0, RESET_CORE_TIMER	# resetCoreTimer()
	syscall				#
while:	li 	$v0, READ_CORE_TIMER	# readCoreTimer()
	syscall				#
	blt 	$v0, 20000, while	# if (readCoreTimer() < 20000) go to while
	bgtz 	$t0, for		# if (ms > 0) go to for
	jr 	$ra