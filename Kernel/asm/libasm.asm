GLOBAL cpuVendor
GLOBAL getKeyPressed
GLOBAL outb
GLOBAL inb
GLOBAL saveRegisters

EXTERN makeBackup
EXTERN getRipBackup

section .text

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
	call getRipBackup				;Obtener RIP
    mov [%1 + 16*8], rax			;Escribir RIP
    mov rax, [rsp+8]  				;RFLAGS
    mov [%1 + 17*8], rax
	
	mov rdi, %1
	call makeBackup
%endmacro	

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

; Puede ir en otro archivo
getKeyPressed:
	push rbp
	mov rbp, rsp

    mov rax, 0

.cicle:
	in al, 64h
    and al, 0x01
    je .cicle
    in al, 60h

	mov rsp, rbp
	pop rbp
    ret

saveRegisters:
	saveRegistersMacro regsArr
	
	ret

outb:
	mov rdx, rdi
	mov rax, rsi
	out dx, al
	ret

inb:
	mov rdx, rdi
	in al, dx
	ret

section .bss
    regsArr resb 144