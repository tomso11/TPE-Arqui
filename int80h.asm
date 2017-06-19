GLOBAL _int80h

section .text

_int80h:
    push rbp
    mov rbp,rsp

    push rbx  ; backups
    push r12
    push r13
    push r15

    mov rax,rdi    ; syscall
    mov rbx,rsi
    push rcx
    mov rcx,rdx
	pop rdx
    int 80h

    pop r15
    pop r13
    pop r12
    pop rbx

    mov rsp,rbp
    pop rbp
    ret