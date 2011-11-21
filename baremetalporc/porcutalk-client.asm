[BITS 64]
[ORG 0x0000000000200000]

%include "bmdev.asm"

start:
  rdtsc				;Time = 64-bit stored in EDX:EAX
  mov ebx, [cycles_in_epoch]
  div ebx			;Divides EDX:EAX by EBX and stores in EDX:EAX
  mov ebx, eax;
  call b_smp_numcores 		; RAX gets number of cores.
  mov rcx, rax        		; move number to ecx
  mov rax, start_thread		; set smp workload pointer
  dec rcx
				;ebx = current epoch, rcx = (numcores-1), rax = start_thread addr
spawn:
  mov [current_epoch + 4*rcx], ebx
  call b_smp_enqueue
  dec rcx
  cmp rcx, 0
  jne spawn

start_thread:
  mov rax, core_mutex		;Assign unique ID's to each task
  call b_smp_lock
  xor rbx, rbx
  mov ebx, [core_num]
  mov r8, rbx
  inc rbx
  mov [core_num], ebx
  mov rax, core_mutex
  call b_smp_unlock
  mov rax, r8			;r8 = unique id [0-(numcores-1)]
  mov rcx,8
  mul rcx
  mov r9, rax			;r9 = 8*id
  mov rcx,4
  mul rcx
  mov r10, rax			;r10 = 32*id
  ;Copy msglen to epochs_left(per thread)
  mov ebx, 0
  mov [num_epochs + 4*r8], ebx

pad_nop:
  rdtsc			     	;Time = 64-bit stored in EDX:EAX
  mov ebx, [cycles_in_epoch]	;Calculate epoch number
  div ebx			;Divides EDX:EAX by EBX and stores in EDX:EAX

  cmp eax, [current_epoch + 4*r8];Detect transition to next epoch
  je pad_nop			;If not yet, pad some more
  mov [current_epoch + 4*r8], eax;Update current epoch

start_loop:
  rdtsc
  shl rdx, 32			;Calculate start time, store in rcx
  xor rax, rdx
  mov [start_time + r9], rax	;Store the start time
  xor rcx, rcx			;Clear rcx
  mov ecx, [num_loops]
  ;DO IT!!!
  mov eax, [current_epoch + 4*r8]
  and eax, 1
  cmp eax, 0
  je loop_miss
loop_take:
  call branch_take
  dec ecx
  cmp ecx, 0
  jne loop_take
  jmp end_loop
loop_miss:
  call branch_leave
  dec ecx
  cmp ecx, 0
  jne loop_miss

end_loop:
  rdtsc				;Print time it took
  shl rdx, 32
  xor rax, rdx

  ;Write iteration Time.
  mov rdx, [start_time + r9]
  sub rax, rdx
  lea rdi, [output_str + r10]
  call b_int_to_string

  ; Write dash.
  mov rsi, dash_str
  lea rdi, [output_str + r10]
  call b_string_append

  ; Write cpu #.
  mov rax, r8
  lea rdi, [buffer_str + r10]
  call b_int_to_string
  lea rsi, [buffer_str + r10]
  lea rdi, [output_str + r10]
  call b_string_append
  
  ; Write dash.
  mov rsi, dash_str
  lea rdi, [output_str + r10]
  call b_string_append
  
  ; Write Epoch #.
  xor rax, rax			;DEBUG Print Current Epoch
  mov eax, [current_epoch + 4*r8]
  lea rdi, [buffer_str + r10]
  call b_int_to_string
  lea rsi, [buffer_str + r10]
  lea rdi, [output_str + r10]
  call b_string_append

  ; Write newline.
  mov rsi, cr_str
  lea rdi, [output_str + r10]
  call b_string_append

  cmp r8, 0			;Only have thread 0 print
  jne end_epoch
  mov rax, 32*8
  call b_print_newline		;Print a separating line

print_loop:
  cmp rax, 0
  je end_epoch
  sub rax, 32
  lea rsi, [output_str + rax]	;parameter for b_print_string
  call b_print_string
  jmp print_loop

end_epoch:
  mov eax, [num_epochs + 4*r8]	;Decrement the current run
  inc eax
  mov [num_epochs + 4*r8], eax
  cmp eax, [msglen]			;Repeat if not done
  jne pad_nop

exit:
  ret

%include "branches.asm"

section .data
  message: db 'Hello World!'
  msglen:  equ     $-message

  cr_str: 	db 13, 0
  dash_str:	db 32, 45, 32, 0
  output_str: 	times 32*8 db 0	; Space for status
  buffer_str:   times 32*8 db 0 ; Space for converting numbers

  core_mutex	times 8 db 0
  core_num	dd 0x00000000
  num_cores	dq 0x00000008
  cycles_in_epoch:dd 0xCAA7E200	;3.4*10^9 cycles = 1 second
  num_loops: 	dd 0x00000100	;Number of loops to run per epoch

section .bss
  current_epoch: resd 8; Current epoch number
  num_epochs:	 resd 8;
  start_time:    resq 8;

