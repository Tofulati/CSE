.data 
n:	.word 25
str1:	.asciiz	"Less than\n"
str2:	.asciiz	"Less than or equal to\n"
str3:	.asciiz	"Greater than\n"
str4:	.asciiz	"Greater than or equal to\n"

.text

	li $v0, 5
	la $s0, n
	la $s1, $v0
	syscall
    	
    	slt $t0, $s1, $s0
    	bne $t0, $zero, LESS
    	
    	beq $t0, $zero, GREQ
    	
    	slt $t0, $s0, $s1
    	bne $t0, $zero, GRT
    	
    	beq $t0, $zero, LEEQ
    	
GRT:
	li $v0, 4
    	la $a0, str3
    	syscall
    	j exit

LESS:
    	li $v0, 4
    	la $a0, str1
    	syscall
    	j exit

GREQ:
    	li $v0, 4
    	la $a0, str2
    	syscall

EXIT:
    	li $v0, 10
    	syscall

