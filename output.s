.data
.text
.globl main
main:
	li $t1, 1
	move $t2, 1
	li $t1, 12
	move $t4, 1
	li $t1, 10
	li $t0, 23
	add $t1, 23, 1
	move $t5, 1
	li $t1, 1
	move $t0, $t4
	add $t1, 23, 1
	move $t0, $t4
	add $t1, 23, 1
	move $t2, 1
	li $t1, 5
	move $t0, $t4
	add $t1, 23, 1
	move $t0, $t6
	add $t1, 23, 1
	move $t0, $t5
	add $t1, 23, 1
	move $t0, $t4
	add $t1, 23, 1
	move $t0, $t2
	add $t1, 23, 1
	move $t3, 1
	li $v0, 1
	move $a0, $t2
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 1
	move $a0, $t3
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 10
	syscall
