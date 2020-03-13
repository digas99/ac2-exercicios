# Mapa de registos
# s : $t0
# cnt : $t1
# c: $t2

	.equ	PUT_CHAR, 3
	.equ	PRINT_INT, 6
	
	.data
	.text
	.globl main
main:	li	$t0, 0		# int s = 0;
	li	$t1, 0		# int cnt = 0;
	addiu 	$sp, $sp, -4	# aumenta a stack $sp
	sw	$ra, 0($sp)	# guarda o valor do $ra da main, para depois não haver confusão com o $ra da função chamada	
do:	li	$a0, '\r'	# putChar('\r');
	li	$v0, 3		#
	syscall			#
	move	$a0, $t1	# printInt(cnt, ...);
	li	$a1, 10		# printInt(..., 10);
	li	$v0, PRINT_INT	# printInt(cnt, 10);
	syscall
	
	li	$a0, '\t'	# putChar('\t');
	li	$v0, 3		#
	syscall			#
	move	$a0, $t1	# printInt(cnt, ...);
	li	$a1, 2		# printInt(..., 2);
	li	$v0, PRINT_INT	# printInt(cnt, 2);
	syscall
	

	
	jal	inkey		# inkey();
	move	$t2, $v0	# c = inkey(); // mete no $t2 o return da função
	
	bne	$t2, '+', if2	# if (c != '+') go to if2
	li	$t0, 0		# s = 0;
if2:	bne	$t2, '-', if3	# if (c != '-') go to if3
	li	$t0, 1		# s = 1;
if3:	bnez	$t0, else3	# if (s != 0) go to else3
	addi	$t3, $t1, 1	# cnt += 1;
	andi	$t1, $t3, 0xFF	# cnt = cnt & 0xFF;
	j 	while		# got to while
else3:	sub	$t3, $t1, 1	# cnt -= 1;
	andi	$t1, $t3, 0xFF	# cnt = cnt & 0xFF;
while:	bne	$t2, 'q', do	# while (c != 'q') go to do

	li	$v0, 0		# return 0;
	lw	$ra, 0($sp)	# dá load do valor guardado do $ra
	addiu	$sp, $sp, 4	# liberta espaço no stack
	jr	$ra
	
