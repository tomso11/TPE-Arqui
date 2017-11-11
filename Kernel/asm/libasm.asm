
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
GLOBAL int_test

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
EXTERN sys_time
EXTERN sys_wait
EXTERN sys_malloc
EXTERN sys_free
EXTERN sys_data_address
EXTERN sys_end
EXTERN sys_yield
EXTERN sys_mutex_op
EXTERN sys_mutex_cl
EXTERN sys_mutex_lock
EXTERN sys_mutex_unlock
EXTERN sys_set_foreground
EXTERN sys_fifo_op
EXTERN sys_fifo_cl
EXTERN sys_kill
EXTERN sys_pid
EXTERN sys_ppid
EXTERN sys_process_info
EXTERN sys_cond_open
EXTERN sys_cond_wait
EXTERN sys_cond_signal
EXTERN sys_cond_broadcast
EXTERN sys_cond_close
EXTERN sys_get_pids
EXTERN sys_get_conds_info
EXTERN sys_get_mutexes_info
EXTERN sys_get_fifos_info
EXTERN sys_write
EXTERN sys_read
EXTERN sys_clear
EXTERN sys_exec

;Scheduler
EXTERN timer_handler
EXTERN next_process
EXTERN irqDispatcher
GLOBAL tick_handler


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
	jp finish
clear:
	cmp eax,5
	jne time
	call sys_clear
	jp finish
time:
	cmp eax,6
	jne exec
	call sys_time
	jp finish
exec:
	cmp eax,7
	jne wait_label
	call sys_exec
	jp finish
wait_label:
	cmp eax,8
	jne reserve
	call sys_wait
	jp finish
reserve:
	cmp eax,9
	jne free
	call sys_malloc
	jp finish
free:
	cmp eax,10
	jne data
	call sys_free
	jp finish
data:
	cmp eax,11
	jne end
	call sys_data_address
	jp finish
end:
	cmp eax,12
	jne end 
	call sys_end
	jp finish
yield:
	cmp eax,13
	jne mutex_op
	call sys_yield
	jp finish
mutex_op:
	cmp eax,14
	jne mutex_cl
	call sys_mutex_op
	jp finish
mutex_cl:
	cmp eax,15
	jne mutex_lo
	call sys_mutex_cl
	jp finish
mutex_lo:
	cmp eax,16
	jne mutex_ul
	call sys_mutex_lock
	jp finish
mutex_ul:
	cmp eax,17
	jne set_fg
	call sys_mutex_unlock
	jp finish
set_fg:
	cmp eax,18
	jne fifo_op
	call sys_set_foreground
	jp finish
fifo_op:
	cmp eax,19
	jne fifo_cl
	call sys_fifo_op
	jp finish
fifo_cl:
	cmp eax,20
	jne kill
	call sys_fifo_cl
	jp finish
kill:
	cmp eax,21
	jne pid
	call sys_kill
	jp finish
pid:
	cmp eax,22
	jne ppid
	call sys_pid
	jp finish
ppid:
	cmp eax,23
	jne get_process_info
	call sys_ppid
	jp finish
get_process_info:
	cmp eax,24
	jne cond_op
	call sys_process_info
	jp finish
cond_op:
	cmp eax,25
	jne cond_wait
	call sys_cond_open
	jp finish
cond_wait:
	cmp eax,26
	jne cond_signal
	call sys_cond_wait
	jp finish
cond_signal:
	cmp eax,27
	jne cond_bc
	call sys_cond_signal
	jp finish
cond_bc:
	cmp eax,28
	jne cond_cl
	call sys_cond_broadcast
	jp finish
cond_cl:
	cmp eax,29
	jne get_current_pids
	call sys_cond_close
	jp finish
get_current_pids:
	cmp eax,30
	jne get_conds_info
	call sys_get_pids
	jp finish
get_conds_info:
	cmp eax,31
	jne get_mutexes_info
	call sys_get_conds_info
	jp finish
get_mutexes_info:
	cmp eax,32
	jne get_fifos_info
	call sys_get_mutexes_info
	jp finish
get_fifos_info:
	cmp eax,33
	jne finish
	call sys_get_fifos_info
	jp finish
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
