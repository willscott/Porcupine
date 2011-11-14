[BITS 64]
[ORG 0x0000000000200000]

%include "bmdev.asm"

start:
	rdtsc			;Time = 64-bit stored in EDX:EAX
	mov ebx, [cycles_in_epoch]
	div ebx			;Divides EDX:EAX by EBX and stores in EDX:EAX
	mov ebx, eax;
	call b_smp_numcores ; RAX gets number of cores.
	mov rcx, rax        ; move number to ecx
  mov rax, pad_nop    ; set smp workload pointer

spawn:
  mov [current_epoch + rcx], ebx
  call b_smp_enqueue
  sub rcx, 1
  cmp rcx, 0
  jne spawn

pad_nop:
  call b_smp_get_id
  mov r8, rax;
  mov rcx,16;
  mul rcx;
  mov r9, rax; 16*proc
  mov rcx,2;
  mul rcx
  mov r10, rax; 32*proc

	rdtsc			     ;Time = 64-bit stored in EDX:EAX
	mov rcx, rax	 ;Save lower 32-bits of time for start_loop
	mov ebx, [cycles_in_epoch];Calculate epoch number
	div ebx			;Divides EDX:EAX by EBX and stores in EDX:EAX

	cmp eax, [current_epoch + 4*r8];Detect transition to next epoch
	je pad_nop		;If not yet, pad some more
	mov [current_epoch + 4*r8], eax;Update current epoch

start_loop:
	rdtsc
	shl rdx, 32		;Calculate start time, store in rcx
	xor rax, rdx
	mov [start_time + r9], rax	;Store the start time
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

  ; Write iteration Time.
	mov rdx, [start_time + r9]
	sub rax, rdx
	mov rdi, [output_str + r10]
	call b_int_to_string

  ; Write dash.
	mov rsi, dash_str
	call b_string_append

  ; Write cpu #.
  mov rax, r8
  mov rdi, [buffer_str + r9]
  call b_int_to_string
  mov rsi, rdi
  mov rdi, [output_str + r10]
  call b_string_append
  
  ; Write dash.
	mov rsi, dash_str
	call b_string_append
  
  ; Write Epoch #.
	xor rax, rax		;DEBUG Print Current Epoch
	mov eax, [current_epoch + 4*r8]
	mov rdi, [buffer_str + r9]
  call b_int_to_string
  mov rsi, rdi
  mov rdi, [output_str + r10]
  call b_string_append

  ; Write newline.
  mov rsi, cr_str
  call b_string_append

	mov rsi, rdi 		;parameter for b_print_string
	call b_print_string

	mov eax, [num_epochs]	;Decrement the current run
	dec eax
	mov [num_epochs], eax
	cmp eax, 0		;Repeat if not done
	jne pad_nop

exit:
	
ret

section .data
  output_str: 	times 32*8 db 0	; Space for status
  buffer_str:   times 16*8 db 0 ; Space for converting numbers
  cr_str: 	db 13, 0
  dash_str:	db 32, 45, 32, 0

  num_cores	dd 0x00000008
  cycles_in_epoch:dd 0xCAA7E200	;3.4*10^9 cycles = 1 second
  num_loops: 	dd 0x00008000	;Number of loops to run per epoch
  num_epochs:	dd 0x0000000a	;Number of epochs to run in program

section .bss
  current_epoch: resd 8; Current epoch number
  start_time:    resq 8;
