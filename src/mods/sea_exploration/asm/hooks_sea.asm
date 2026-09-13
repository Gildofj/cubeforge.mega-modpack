; ==============================================================================
; cubeforge-sea-exploration - MASM x64 Hooks
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

EXTERN OnChestInteraction:PROC

EXTERN ASMIsChestOpen_jmpback:QWORD
EXTERN ASMIsChestOpen_bail:QWORD
EXTERN ASMChestInteractionHandler_jmpback:QWORD

PUBLIC ASMIsChestOpen
PUBLIC ASMChestInteractionHandler

ASMIsChestOpen PROC
    cmp byte ptr [rdi + 19Ch], 2
    jne @F
    mov rax, QWORD PTR [ASMIsChestOpen_bail]
    jmp rax
@@:
    mov rax, QWORD PTR [rsi + 8]
    mov rcx, QWORD PTR [rax + 448h]
    mov rax, QWORD PTR [rdi + 10h]
    mov rdx, QWORD PTR [rdi + 18h]
    mov r8,  QWORD PTR [rdi + 20h]
    sub rax, QWORD PTR [rcx + 10h]
    sub rdx, QWORD PTR [rcx + 18h]
    sub r8,  QWORD PTR [rcx + 20h]
    mov r11, QWORD PTR [ASMIsChestOpen_jmpback]
    jmp r11
ASMIsChestOpen ENDP

ASMChestInteractionHandler PROC
    PUSH_ALL
    mov rdx, rdi
    mov rcx, rsi
    PREPARE_STACK
    call OnChestInteraction
    RESTORE_STACK
    POP_ALL
    mov rax, QWORD PTR [ASMChestInteractionHandler_jmpback]
    jmp rax
ASMChestInteractionHandler ENDP

END
