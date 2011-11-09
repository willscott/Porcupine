[BITS 64]
[ORG 0x0000000000200000]

%include "bmdev.asm"

start:
	mov rcx, [num_loops]		;Number of loops
	rdtsc			;START TIME = 64-bit stored in EDX:EAX
	shl rdx, 32
	xor rax, rdx
	mov rbx, rax		;Store start time in rbx

loop:
	dec rcx
	cmp rcx, 0
	jg loop

exit:
	rdtsc			;STOP TIME
	shl rdx, 32
	xor rax, rdx
	
	sub rax, rbx		;Total time
	mov rdi, output_str	;string parameter for b_int_to_string
	mov rsi, rdi 		;parameter for b_print_string
	call b_int_to_string
	call b_print_string
	mov rsi, cr_str		;Carriage Return
	call b_print_string

ret

output_str: times 21 db 0	;Space for printing time
cr_str: db 13, 0
num_loops: dq 0x0000000000000400
