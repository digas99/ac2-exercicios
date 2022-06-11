	.equ SFR_BASE_HI, 0xBF88
	.equ TRISE, 0x6100
	.equ PORTE, 0x6110
	.equ LATE, 0x6120
	.equ TRISB, 0x6040
	.equ PORTB, 0x6050
	.equ LATB, 0x6060
	
	.data
	.text
	.globl main
main:
	lui $t0, SFR_BASE_HI	#
							# // Set RE0 as output
	lw $t1, TRISE($t0)		# $t1 = TRISE
	andi $t1, $t1, 0xFFFE	# RE0 = 0
	sw $t1, TRISE($t0)		# update TRISE
	
							# // Set RB0 as input
	lw $t1, TRISB($t0)		# $t1 = TRISB
	ori $t1, $t1, 1			# RB0 = 1
	sw $t1, TRISB($t0)		# update TRISB
	
while:						# while(1) {
							#	// Get value from RB0
	lw $t1, PORTB($t0)		#	$t1 = PORTB
	andi $t1, $t1, 1		#	isolate value from RB0
							#	// Set value from RB0 to RE0
	lw $t2, LATE($t0)		#	$t2 = LATE
	andi $t2, $t2, 0xFFFE	#	// Reset value of RE0
	or $t2, $t2, $t1		#	RE0 = RB0
	sw $t2, LATE($t0)		#	update value of RE0
	j while					# }
	jr $ra
	
	