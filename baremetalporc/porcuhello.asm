[BITS 64]
[ORG 0x0000000000200000]

%include "bmdev.asm"

start:
	mov rsi, test_message
	call b_print_string

ret

test_message: db 'Porc says hello!', 13, 0
