# a) Determine o valor da constante "K", de modo a que para " ms " igual a 1
# o atraso gerado seja de 1ms.
# 10ms -> 200000
# 1ms -> 20000
# logo, k = 20000	
	
	.equ READ_CORE_TIMER, 11
	.equ RESET_CORE_TIMER, 12
	.equ K, 20000
	
	.data
	.text
	.globl delay
delay:
	li $v0, RESET_CORE_TIMER		# resetCoreTimer()
	syscall							#
	mul $t0, $a0, K					# // $t0 = k * ms
delay_while:						# while(readCoreTimer() < K * ms)
	li $v0, READ_CORE_TIMER			#
	syscall							#
	blt $v0, $t0, delay_while		#
	jr $ra