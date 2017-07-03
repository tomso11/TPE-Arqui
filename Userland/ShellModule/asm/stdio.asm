GLOBAL readC
GLOBAL writeC
GLOBAL sys_call
GLOBAL clearC

section .text

; Se llaman desde syscalls.c, y realizan la interrupcion 80h que sera atendida por la irq80Handler en libasm

readC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax, 3
	mov ebx,0 ;siempre en pantalla
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret
writeC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,4
	mov ebx,0 ;siempre en pantalla
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

clearC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,5
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

;Syscall generico

sys_call:
	push rbp
	mov rbp, rsp
	mov rcx, rdx
	mov rax,rdi
	mov ebx,0 ;siempre en pantalla
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret
