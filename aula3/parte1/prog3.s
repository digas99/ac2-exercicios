	.equ	SFR_BASE_HI, 0xBF88
	.equ	TRISE, 0x6100
	.equ	PORTE, 0x6110
	.equ	LATE, 0x6120
	.equ	TRISB, 0x6040
	.equ	PORTB, 0x6050
	.equ	LATB, 0x6060
	.equ	idshfi, 0xBF886050
	.equ	PRINT_INT, 6
	.data
	.text
	.globl main
main:	lui	$t1,SFR_BASE_HI
					# configuração dos portos RE0-3 como saída	
	lw	$t2, TRISE($t1)		# O endereço atribuído ao registo TRISE é 0xBF886100
					# (Mem_addr = 0xBF880000 + 0x6100)
	andi	$t2, $t2, 0xFFF0	# Modificação, bit0-3 = 0 (TRISE AND 1111111111110000)
	sw	$t2, TRISE($t1)		# "Dar update das modificações)
					
					# configuração dos portos RB0-3 como entrada
	lw	$t2, TRISB($t1)		# O endereço atribuído ao registo TRISB é 0xBF886040
					# (Mem_addr = 0xBF880000 + 0x6040)
	ori	$t2, $t2, 15		# (TRISB OR 0000 0000 0000 1111)
	sw	$t2, TRISB($t1)		# "Dar update das modificações)
					
while:	
					# leitura do valor do porto de entrada
	lw	$t2, PORTB($t1)
	nor	$t3, $t2, $t2
	andi	$t2, $t2, 0x0006
	andi	$t3, $t3, 0x0009
	or	$t2, $t2, $t3		
					# escrever esse valor no porto de saída
	lw	$t3, LATE($t1)
	andi	$t3, $t3, 0x0000	# resetar o valor dos 4 bits menos significativos
	or	$t2, $t2, $t3		# juntar os valores a modificar nos 4 bits, com o resto dos bits do LATB
		
	sw	$t2, LATE($t1)		# escrever no LATE o novo valor
	j	while
	
	jr	$ra
