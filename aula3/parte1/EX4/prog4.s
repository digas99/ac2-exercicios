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
	li $t0, SFR_BASE_HI
							# Set RE0-RE3 as outputs
	lw $t1, TRISE($t0)		# $t1 = TRISE
	andi $t1, $t1, 0xFFF0	# RE0-RE3 = 0
	sw $t1, TRISE($t0)		# update TRISE
							# Set RB0-RB3 as inputs
	lw $t1, TRISB($t0)		# $t1 = TRISB
	ori $t1, $t1, 0x000F	# RB0-RB3 = 1
	sw $t1, TRISB($t0)		# update TRISB
	
while:						# while(1) {
	lw $t1, PORTB($t0)		#	$t1 = PORTB
	
	j while					# }