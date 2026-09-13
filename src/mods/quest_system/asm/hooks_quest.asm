; ==============================================================================
; cubeforge-quest-system - MASM x64 Hooks
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

EXTERN OnGetItemDescription:PROC
EXTERN OnGetItemName:PROC

EXTERN ASMOnGetItemDescription_jmpback:QWORD
EXTERN ASMOnGetItemDescription_jmpback_2:QWORD
EXTERN ASMOnGetItemName_jmpback:QWORD
EXTERN ASMOnGetItemName_bail:QWORD

PUBLIC ASMOnGetItemDescription
PUBLIC ASMOnGetItemName

ASMOnGetItemDescription PROC
    movss DWORD PTR [rsi + 1B0h], xmm13
    PUSH_ALL
    mov rcx, rsi
    mov rdx, r14
    mov r8, rdi
    PREPARE_STACK
    call OnGetItemDescription
    RESTORE_STACK
    cmp rax, 1
    je Label_ItemDesc_Bail
    POP_ALL
    cmp byte ptr [r14], 1
    jnz Label_ItemDesc_Jmp2
    mov rax, QWORD PTR [ASMOnGetItemDescription_jmpback]
    jmp rax
Label_ItemDesc_Jmp2:
    mov rax, QWORD PTR [ASMOnGetItemDescription_jmpback_2]
    jmp rax
Label_ItemDesc_Bail:
    POP_ALL
    movss xmm0, xmm10
    mov rax, QWORD PTR [ASMOnGetItemDescription_jmpback_2]
    jmp rax
ASMOnGetItemDescription ENDP

ASMOnGetItemName PROC
    PUSH_ALL
    mov rdx, r8
    lea r8, [rbp + 1E0h]
    PREPARE_STACK
    call OnGetItemName
    RESTORE_STACK
    cmp rax, 1
    je Label_ItemName_Bail
    POP_ALL
    mov rsi, rdx
    mov QWORD PTR [rbp + 40h], rdx
    mov rdi, rcx
    mov QWORD PTR [rbp + 160h], rcx
    mov rax, QWORD PTR [ASMOnGetItemName_jmpback]
    jmp rax
Label_ItemName_Bail:
    POP_ALL
    mov rsi, rdx
    mov QWORD PTR [rbp + 40h], rdx
    mov rdi, rcx
    mov QWORD PTR [rbp + 160h], rcx
    mov rax, QWORD PTR [ASMOnGetItemName_bail]
    jmp rax
ASMOnGetItemName ENDP

END
