.data
.text
.globl main
main:
	li $t1, 1
	move $t2, $t1
	li $t1, 12
	move $t4, $t1
	li $t1, 10
	li $t0, 23
	add $t1, $t0, $t1
	move $t5, $t1
	li $t1, 1
	move $t0, $t4
	add $t1, $t0, $t1
	move $t0, $t4
	add $t1, $t0, $t1
	move $t2, $t1
	li $t1, 5
	move $t0, $t4
	add $t1, $t0, $t1
	move $t0, $t6
	add $t1, $t0, $t1
	move $t0, $t5
	add $t1, $t0, $t1
	move $t0, $t4
	add $t1, $t0, $t1
	move $t0, $t2
	add $t1, $t0, $t1
	move $t3, $t1
	move $t1, $t2
	move $t0, $t3
	li $v0, 10
	syscall
