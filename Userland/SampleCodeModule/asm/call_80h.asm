section .text
global call_80h

call_80h:
    mov rax, 3
    mov rbx, 1
    mov rcx, buffer
    mov rdx, 100
    int 80h

    push rax

    mov rax, 4
    mov rbx, 1
    mov rcx, buffer
    mov rdx, [rsp+8]
    int 80h

    mov rax, 4
    mov rbx, 2
    mov rcx, buffer
    mov rdx, [rsp+8]
    int 80h

    pop rax

    ret

section .rodata
stdout db "STDOUT", 0
stdoutsize equ $-stdout
stderr db "STDERR", 0
stderrsize equ $-stderr

section .bss
buffer resb 100