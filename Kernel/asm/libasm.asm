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
GLOBAL sys_callHandler
EXTERN sys_write
EXTERN sys_read
EXTERN sys_clear
;EXTERN sys_call_echoC
;EXTERN sys_call_runC
GLOBAL cli

GLOBAL set_cursor
GLOBAL cursor

EXTERN irqDispatcher

%include "./asm/macros.m"

section .text

irq0Handler:
	irqHandler 0
	
irq1Handler:
	irqHandler 1

irq12Handler:
	irqSlaveHandler 2

irq80Handler:
	jp sys_callHandler

cursor:
	pushaq
	push rbp
	mov rbp, rsp

	mov ah, 0x02
	mov bh, 0
	mov dh, 0
	mov dl, 0

	int 10h

	mov rsp,rbp
	pop rbp
	popaq
	ret

sti:
	sti
	ret

cli:
	cli
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

sys_callHandler:
	cli
	push rbp
	mov rbp, rsp
	cmp eax, 3
	mov rdi,rbx
	mov rsi,rcx
	mov rdx,rdx
	jne write
	call sys_read
	jp finish
write:
	cmp eax,4
	jne clear
	call sys_write
clear:
	cmp eax,5
	jne finish
	call sys_clear
;echo:
;	cmp eax,6
;	jne run
;	mov rdi,rcx
;	call sys_call_echoC
;run:
;	cmp eax, 7
;	jne finish
;	mov rdi,rcx
;	call sys_call_runC
finish:
	mov rdi,rax
	mov al, 20h
	out 20h, al
	mov rax,rdi	
	mov rsp, rbp
	pop rbp
	sti
	iretq


; Set cursor position (text mode 80x25)
; @param BL The row on screen, starts from 0
; @param BH The column on screen, starts from 0
;=============================================================================
set_cursor:     pushaq
 
                ;unsigned short position = (row*80) + col;
                ;AX will contain 'position'
                mov ax,bx
                and ax,0ffh             ;set AX to 'row'
                mov cl,80   
                mul cl                  ;row*80
 
                mov cx,bx               
                shr cx,8                ;set CX to 'col'
                add ax,cx               ;+ col
                mov cx,ax               ;store 'position' in CX
 
                ;cursor LOW port to vga INDEX register
                mov al,0fh             
                mov dx,3d4h             ;VGA port 3D4h
                out dx,al             
 
                mov ax,cx               ;restore 'postion' back to AX  
                mov dx,3d5h             ;VGA port 3D5h
                out dx,al               ;send to VGA hardware
 
                ;cursor HIGH port to vga INDEX register
                mov al,0eh
                mov dx,3d4h             ;VGA port 3D4h
                out dx,al
 
                mov ax,cx               ;restore 'position' back to AX
                shr ax,8                ;get high byte in 'position'
                mov dx,3d5h             ;VGA port 3D5h
                out dx,al               ;send to VGA hardware
 
                popaq
                ret
