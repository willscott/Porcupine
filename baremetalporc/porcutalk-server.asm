[BITS 64]
[ORG 0x0000000000200000]

%include "bmdev.asm"

start:
	call b_smp_numcores
	rdtsc			;Time = 64-bit stored in EDX:EAX
	mov ebx, [cycles_in_epoch]
	div ebx			;Divides EDX:EAX by EBX and stores in EDX:EAX
	mov [current_epoch], eax

pad_nop:
	rdtsc			;Time = 64-bit stored in EDX:EAX
	mov rcx, rax		;Save lower 32-bits of time for start_loop
	mov ebx, [cycles_in_epoch];Calculate epoch number
	div ebx			;Divides EDX:EAX by EBX and stores in EDX:EAX

	cmp eax, [current_epoch];Detect transition to next epoch
	je pad_nop		;If not yet, pad some more
	mov [current_epoch], eax;Update current epoch

start_loop:
	rdtsc
	shl rdx, 32		;Calculate start time, store in rcx
	xor rax, rdx
	mov [start_time], rax	;Store the start time
	xor rcx, rcx		;Clear rcx
	mov ecx, [num_loops]

loop:
	dec ecx
	cmp ecx, 0
	jne loop
	
end_loop:
	rdtsc			;Print time it took
	shl rdx, 32
	xor rax, rdx
	mov rdx, [start_time]
	sub rax, rdx
	mov rdi, output_str	;string parameter for b_int_to_string
	mov rsi, rdi 		;parameter for b_print_string
	call b_int_to_string
	call b_print_string

	mov rsi, dash_str	;DEBUG Print dash
	call b_print_string

	xor rax, rax		;DEBUG Print Current Epoch
	mov eax, [current_epoch]
	mov rdi, output_str	;string parameter for b_int_to_string
	mov rsi, rdi 		;parameter for b_print_string
	call b_int_to_string
	call b_print_string

	mov rsi, cr_str		;DEBUG Print Carriage Return
	call b_print_string

	mov eax, [num_epochs]	;Decrement the current run
	dec eax
	mov [num_epochs], eax
	cmp eax, 0		;Repeat if not done
	jne pad_nop

exit:
	
ret

output_str: 	times 21 db 0	;Space for printing time
cr_str: 	db 13, 0
dash_str:	db 32, 45, 32, 0

cycles_in_epoch:dd 0xCAA7E200	;3.4*10^9 cycles = 1 second
num_loops: 	dd 0x00000800	;Number of loops to run per epoch
num_epochs:	dd 0x0000000a	;Number of epochs to run in program

current_epoch:	dd 0x00000000	;Current epoch number
start_time:	dq 0x0000000000000000
