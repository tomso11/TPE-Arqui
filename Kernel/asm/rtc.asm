GLOBAL seconds
GLOBAL minutes
GLOBAL hour

section .text

extract:
	push rbp
	mov rbp,rsp
	push rax
	mov al, 0Bh
	out 70h, al
	in al, 71h
	or al, byte 00000100b
	out 71h, al
	pop rax
	out 70h, al
	in  al, 71h
	movzx rax, al
	mov rsp,rbp
	pop rbp
	ret

seconds:
	push rbp
	mov rbp,rsp

	mov al, 0
	call extract

	mov rsp,rbp
	pop rbp

	ret

minutes:
	push rbp
	mov rbp,rsp

	mov al, 2
	call extract

	mov rsp,rbp
	pop rbp

	ret

hour:
	push rbp
	mov rbp,rsp

	mov al, 4
	call extract

	mov rsp,rbp
	pop rbp

	ret
