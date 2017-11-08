GLOBAL readC
GLOBAL writeC
GLOBAL sys_call
GLOBAL clearC
GLOBAL timeC
GLOBAL execC
GLOBAL waitC
GLOBAL reserveC
GLOBAL freeC
GLOBAL dataC
GLOBAL endC
GLOBAL yieldC
GLOBAL mutex_opC
GLOBAL mutex_clC
GLOBAL mutex_loC
GLOBAL mutex_ulC
GLOBAL set_fgC
GLOBAL fifo_opC
GLOBAL fifo_clC
GLOBAL killC
GLOBAL pidC
GLOBAL ppidC
GLOBAL get_process_infoC
GLOBAL cond_opC
GLOBAL cond_waitC
GLOBAL cond_signalC
GLOBAL cond_bcC
GLOBAL cond_clC
GLOBAL get_current_pidsC
GLOBAL get_conds_infoC
GLOBAL get_mutexes_infoC
GLOBAL get_fifos_infoC


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

timeC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,6
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

execC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,7
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

waitC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,8
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

reserveC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,9
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

freeC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,10
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

dataC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,11
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

endC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,12
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

yieldC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,13
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

mutex_opC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,14
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

mutex_clC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,15
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

mutex_loC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,16
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

mutex_ulC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,17
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

set_fgC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,18
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

fifo_opC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,19
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

fifo_clC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,20
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

killC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,21
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

pidC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,22
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

ppidC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,23
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

get_process_infoC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,24
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

cond_opC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,25
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

cond_waitC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,26
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

cond_signalC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,27
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

cond_bcC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,28
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

cond_clC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,29
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

get_current_pidsC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,30
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

get_conds_infoC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,31
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

get_mutexes_infoC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,32
	mov rdx, rsi
	int 80h
	mov rsp, rbp
	pop rbp
	ret

get_fifos_infoC:
	push rbp
	mov rbp, rsp
	mov rcx, rdi
	mov eax,33
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
