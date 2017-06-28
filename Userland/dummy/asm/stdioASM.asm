
GLOBAL readC
GLOBAL writeC
GLOBAL sys_call

section .text

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