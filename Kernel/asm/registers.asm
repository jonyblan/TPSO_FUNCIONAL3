GLOBAL getRAX
GLOBAL getRBX
GLOBAL getRCX
GLOBAL getRDX
GLOBAL getRSI
GLOBAL getRDI
GLOBAL getRSP
GLOBAL getRBP
GLOBAL getR8
GLOBAL getR9
GLOBAL getR10
GLOBAL getR11
GLOBAL getR12
GLOBAL getR13
GLOBAL getR14
GLOBAL getR15

section .text

getRAX:
	ret

getRBX:

	push rbp
    mov rbp, rsp

	mov RAX,RBX

	mov rsp, rbp
    pop rbp	
    ret

getRCX:

	push rbp
    mov rbp, rsp

	mov RAX,RCX

	mov rsp, rbp
    pop rbp	
    ret

getRDX:

	push rbp
    mov rbp, rsp

	mov RAX,RDX

	mov rsp, rbp
    pop rbp	
    ret


getRSI:

	push rbp
    mov rbp, rsp

	mov RAX,RSI

	mov rsp, rbp
    pop rbp	
    ret

getRDI:

	push rbp
    mov rbp, rsp

	mov RAX,RDI

	mov rsp, rbp
    pop rbp	
    ret

getRSP:

	push rbp
    mov rbp, rsp

	mov RAX,RSP

	mov rsp, rbp
    pop rbp	
    ret

getRBP:

	push rbp
    mov rbp, rsp

	mov RAX,[RBP + 8]

	mov rsp, rbp
    pop rbp	
    ret


getR8:

	push rbp
    mov rbp, rsp

	mov RAX,R8

	mov rsp, rbp
    pop rbp	
    ret

getR9:

	push rbp
    mov rbp, rsp

	mov RAX,R9

	mov rsp, rbp
    pop rbp	
    ret

getR10:

	push rbp
    mov rbp, rsp

	mov RAX,R10

	mov rsp, rbp
    pop rbp	
    ret

getR11:

	push rbp
    mov rbp, rsp

	mov RAX,R11

	mov rsp, rbp
    pop rbp	
    ret

getR12:

	push rbp
    mov rbp, rsp

	mov RAX,R12

	mov rsp, rbp
    pop rbp	
    ret

getR13:

	push rbp
    mov rbp, rsp

	mov RAX,R13

	mov rsp, rbp
    pop rbp	
    ret

getR14:

	push rbp
    mov rbp, rsp

	mov RAX,R14

	mov rsp, rbp
    pop rbp	
    ret

getR15:

	push rbp
    mov rbp, rsp

	mov RAX,R15

	mov rsp, rbp
    pop rbp	
    ret
