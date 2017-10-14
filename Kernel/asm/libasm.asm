GLOBAL cpuVendor
GLOBAL run_mod

;interruptions
GLOBAL sti
GLOBAL cli
GLOBAL hlt
GLOBAL setPicMaster
GLOBAL setPicSlave
GLOBAL irq0Handler
GLOBAL irq1Handler
GLOBAL irq12Handler

;syscalls
GLOBAL irq80Handler
GLOBAL read_port
GLOBAL write_port
GLOBAL sys_callHandler

;scheduler
GLOBAL _change_process
GLOBAL _yield_process
GLOBAL _yield_interrupt
GLOBAL _unlocked


;para mouse
GLOBAL set_cursor
GLOBAL cursor

;paginacion
GLOBAL cli
GLOBAL saveCR3
GLOBAL page_enable

;Syscalls
EXTERN sys_write
EXTERN sys_read
EXTERN sys_clear
;EXTERN sys_call_echoC
;EXTERN sys_call_runC

;Scheduler
EXTERN timer_handler
EXTERN next_process
EXTERN irqDispatcher


%include "./asm/macros.m"

section .text

; Estos handlers llaman al irqDispatcher en "interruptions.c" con el parametro para elegir el handler que se requiere

irq0Handler:
	irqHandler 0
	
irq1Handler:
	irqHandler 1

irq12Handler:
	irqSlaveHandler 2

irq80Handler:
	jp sys_callHandler

;scheduling

tick_handler:
	pushaq

	call timer_handler

	mov rdi, rsp
	call next_process

	mov rsp, rax

	mov al, 0x20
	out 0x20, al

	popaq

	iretq

_change_process:
	mov rsp, rdi ;cambiar el sp al del nuevo proc
	popaq
	iretq


_yield_process:
	int 70h   ; llamo al timer tick     	
	ret

_yield_interrupt:
	pushaq

	mov rdi, rsp
	call next_process

	mov rsp, rax
	popaq

	iretq

_unlocked:
  mov rax, 0
  xchg rax, [rdi]
	ret


; Nos permite correr un modulo con su direcc en rax

run_mod:
	push rbp
	mov rbp, rsp

	jmp rax ;en rax estara la direccion en hx del modulo a correr

	mov rsp, rbp
	pop rbp
	ret

; Dibuja el cursor en pantalla

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

;Habilitar/deshabilitar las interrupciones

sti:
	sti
	ret

cli:
	cli
	ret

hlt:
	sti
	hlt
	ret

;Seteo de PIC, provisto por la catedra
	
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

;Leer y escribir en un puerto

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


saveCR3:

	mov rax, cr3
	mov cr3, rax
	ret

;osdev habilitar paginas

page_enable:
	mov rax, 0x10000  ;PD base
	mov cr3,rax

	mov rax,cr0
	or rax, 0x8000001
	mov cr0,rax

	ret

; nos permite manejar el flujo de syscalls mediante el registro eax.

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


;obtenido desde Pure64

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

