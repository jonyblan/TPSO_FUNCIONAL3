
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _irq60Handler

GLOBAL _exception00Handler
GLOBAL _exception06Handler

GLOBAL getRipBackup

EXTERN irqDispatcher
EXTERN syscallDispatcher
EXTERN exceptionDispatcher
EXTERN saveRegisters
EXTERN getStackBase
EXTERN makeBackup
EXTERN schedule

SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro irqHandlerMaster 1
	push rdi
	mov rdi, [rsp + 8]
	mov [ripBackup], rdi
	pop rdi
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro

%macro saveRegistersMacro 1
	mov [%1], rax
	mov [%1 + 1*8], rbx
	mov [%1 + 2*8], rcx
	mov [%1 + 3*8], rdx
	mov [%1 + 4*8], rsi
	mov [%1 + 5*8], rdi
	mov [%1 + 6*8], rbp
	mov [%1 + 7*8], r8
	mov [%1 + 8*8], r9
	mov [%1 + 9*8], r10
	mov [%1 + 10*8], r11
	mov [%1 + 11*8], r12
	mov [%1 + 12*8], r13
	mov [%1 + 13*8], r14
	mov [%1 + 14*8], r15
	mov [%1 + 15*8], rsp
	;mov rax, [rsp]    				;RIP --> CORREGIR
	mov rax, [ripBackup]
    mov [%1 + 16*8], rax
    mov rax, [rsp+8]  				;RFLAGS
    mov [%1 + 17*8], rax
	
	mov rdi, %1
	call makeBackup
%endmacro

%macro exceptionHandler 1
	push rdi
	mov rdi, [rsp + 8]
	mov [ripBackup], rdi
	pop rdi
	saveRegistersMacro regsExcArr

	pushState

	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

	popState

	call getStackBase

	mov [rsp+ 24], rax
	mov rax, userland
	mov [rsp],rax

	iretq

%endmacro


_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	;irqHandlerMaster 0
	pushState

	mov rdi, 0
	call irqDispatcher

	mov rdi, rsp
	call schedule
	mov rsp, rax

	;signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq


;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5

;System calls
_irq60Handler:
	push rbx
	push rdi
	push rsi
	mov rdi, rax
	mov rsi, rbx
	push rdx		; swap entre
	mov rdx, rcx	; los registros
	pop rcx			; rcx y rdx

	;push rdi
	;mov rdi, 0xFF
	;call picMasterMask
	;pop rdi

	call syscallDispatcher
	
	;mov rdi, 0xFD
	;call picMasterMask

	pop rsi
	pop rdi
	pop rbx

	iretq

;Zero Division Exception
_exception00Handler:
	exceptionHandler 0

;Invalid Opcode Exception
_exception06Handler:
	exceptionHandler 6

haltcpu:
	cli
	hlt
	ret

getRipBackup:
	mov rax, [ripBackup]
	ret

section .rodata
	userland equ 0x400000

section .bss
	aux resq 1
	regsExcArr resb 144
	ripBackup resb 8