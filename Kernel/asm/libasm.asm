GLOBAL cpuVendor
GLOBAL sti
GLOBAL setPicMaster
GLOBAL setPicSlave
GLOBAL irq0Handler
GLOBAL irq1Handler
GLOBAL irq12Handler
GLOBAL irq80Handler
GLOBAL read_port
GLOBAL write_port
GLOBAL keyboard

EXTERN irqDispatcher

%include "./asm/macros.m"

section .text

irq0Handler:
	irqHandler 0
	
irq1Handler:
	; Keyboard interrupt. IRQ 0x01, INT 0x21
; This IRQ runs whenever there is input on the keyboard
align 16
keyboard:
	push rdi
	push rax

	xor eax, eax

	in al, 0x60			; Get the scancode from the keyboard
	test al, 0x80
	jnz keyboard_done

	mov [0x000B8088], al		; Dump the scancode to the screen


keyboard_done:
	mov al, 0x20			; Acknowledge the IRQ
	out 0x20, al

	pop rax
	pop rdi
	iretq
	;irqHandler1

irq12Handler:
	irqHandler 2

irq80Handler:
	irqHandler 3

sti:
	sti
	ret
	
setPicMaster:
	push rbp
	mov rbp, rsp
	
	mov rax, rdi
	out 21h, al
 	
	mov rsp, rbp
	pop rbp
	ret

setPicSlave:
	push rbp
	mov rbp, rsp

	mov rax, rdi
	out 0xa1, al

	mov rsp, rbp
	pop rbp
	ret

read_port:
	push rbp
	mov rbp, rsp
	mov rdx, rdi
	in al, dx
	mov rsp, rbp
	pop rbp
	ret

write_port:
	push rbp
	mov rbp, rsp
	mov rdx, rdi
	mov rax, rsi
	out dx, al
	mov rsp, rbp
	pop rbp
	ret

cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

