; ==============================================================================
; cubeforge-region-lock - MASM x64 Hooks
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

EXTERN OnGetEffectiveItemRarity:PROC
EXTERN GetDropRegion:PROC
EXTERN ASMPatchItemDrop_jmpback:QWORD

PUBLIC ASMEffectiveItemRarity
PUBLIC ASMPatchItemDrop

ASMEffectiveItemRarity PROC
    push r15
    PUSH_ALL
    mov rdx, rax
    mov rcx, r9
    PREPARE_STACK
    call OnGetEffectiveItemRarity
    RESTORE_STACK
    mov QWORD PTR [rsp], rax
    POP_ALL
    mov rax, r15
    pop r15
    ret
ASMEffectiveItemRarity ENDP

ASMPatchItemDrop PROC
    push r15
    PUSH_ALL
    PREPARE_STACK
    call GetDropRegion
    RESTORE_STACK
    mov QWORD PTR [rsp], rax
    POP_ALL
    mov rax, r15
    pop r15
    mov rcx, rax
    mov r11, QWORD PTR [ASMPatchItemDrop_jmpback]
    jmp r11
ASMPatchItemDrop ENDP

END
