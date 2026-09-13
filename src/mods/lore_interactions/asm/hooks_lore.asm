; ==============================================================================
; cubeforge-lore-interactions - MASM x64 Hooks
; ==============================================================================
.code

PUSH_ALL MACRO
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
ENDM

POP_ALL MACRO
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
ENDM

PREPARE_STACK MACRO
    mov rax, rsp
    and rsp, -16
    push rax
    sub rsp, 28h
ENDM

RESTORE_STACK MACRO
    add rsp, 28h
    pop rsp
ENDM

EXTERN OnPrintRGBA:PROC
EXTERN ASMIncreaseLore_jmpback:QWORD

PUBLIC ASMIncreaseLore

ASMIncreaseLore PROC
    PUSH_ALL
    mov rcx, rdx
    PREPARE_STACK
    call OnPrintRGBA
    RESTORE_STACK
    POP_ALL
    mov rbx, rdx
    mov r13, rcx
    xor edi, edi
    mov r15d, edi
    mov QWORD PTR [rsp + 70h], rdi
    mov QWORD PTR [rsp + 78h], rdi
    mov rax, QWORD PTR [ASMIncreaseLore_jmpback]
    jmp rax
ASMIncreaseLore ENDP

END
