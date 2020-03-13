	.equ	READ_CORE_TIMER, 11
	.equ	RESET_CORE_TIMER, 12
	.equ	PUT_CHAR, 3
	.equ	PRINT_INT, 6
	.data
	.text
	.globl main
main:	li 	$t0, 0			# counter = 0
					# while(1) {
while:	li 	$v0, READ_CORE_TIMER	# 	readCoreTimer()
	syscall				#
	blt 	$v0, 200000, while	#	if (readCoreTimer() > 200000) go to while
					# 		100 Hz -> 200000
					#		10 Hz -> 20000
					#		5 Hz -> 10000
					#		1 Hz -> 2000
	li 	$v0, RESET_CORE_TIMER	#	resetCoreTimer()
	syscall				#
	li 	$a0, ' '		# 	putChar(' ');
	li 	$v0, PUT_CHAR		#
	syscall				#
	addi 	$t0, $t0, 1		#	counter++
	move 	$a0, $t0		#	printInt(counter, ...)
	li 	$a1, 10			#	printInt(..., 10)
	li 	$v0, PRINT_INT		#	printInt(counter, 10)
	syscall				#
	j 	while			# }
	jr 	$ra 
