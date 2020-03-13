	.equ SFR_BASE_HI, 0xBF88	# 16 MSbits of SFR area
	.equ TRISE, 0x6100		# TRISE adress is 0xBF886100
	.equ PORTE, 0x6110		# PORTE adress is 0xBF886110
	.equ LATE, 0x6120 		# LATE adress is 0xBF886120
	.equ READ_CORE_TIMER, 11
	.equ RESET_CORE_TIMER, 12
	.data
	.text
	.globl main
main:	addu 	$sp, $sp, -4
        sw 	$ra, 0($sp)
	
	lui	$t1, SFR_BASE_HI
	lw	$t2, TRISE($t1)		# READ (Mem_addr = 0xBF880000 + 0x6100)
	li	$t3, 0			# v = 0
	andi	$t2, $t2, 0xFFFE	# TRISE0 = 0	// TRISE && 1111 1111 1111 1110
					#		// configura porto RE0 como saída
	sw	$t2, TRISE($t1)		# update value of TRISE
	
while:	lw	$t2, LATE($t1)		# READ (Mem_addr = 0xBF880000 + 0x6120)
	andi	$t2, $t2, 0xFFFE	# reset do valor do bit 0 do LATE
	or	$t2, $t2, $t3		# LATE0 = v	// TRISE or v
					#		// xxxx xxxx xxxx xxx0 or v = xxxx xxxx xxxx xxxv
					#		// Escreve v no bit 0 do porto E
	sw	$t2, LATE($t1)		# update value of LATE
	
	li	$a0, 10			# delay(10)
	jal	delay			#
	
	nor	$t3, $t3, $t3		# v nor v = !v	// complementa o bit 0 de v
	
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
