; ==============================================================================
; cubeforge-shop-updates - MASM x64 Hooks
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

EXTERN OnShopInteractionHandler:PROC
EXTERN ShopInteractionSub1:PROC
EXTERN ShopInteractionSub2:PROC
EXTERN OnItemPriceHandler:PROC
EXTERN callPowf:PROC

EXTERN ASMShopInteraction_jmpback:QWORD
EXTERN ASMItemPrice_jmpback:QWORD

PUBLIC ASMShopInteractionHandler
PUBLIC ASMItemPriceHandler

ASMShopInteractionHandler PROC
    PUSH_ALL
    mov r9, QWORD PTR [rsi + 1E98h]
    mov r8, QWORD PTR [rsi + 1EA0h]
    mov rdx, rdi
    mov rcx, rsi
    PREPARE_STACK
    call OnShopInteractionHandler
    RESTORE_STACK
    POP_ALL
    mov edx, 2
    mov rcx, rdi
    PREPARE_STACK
    call ShopInteractionSub1
    RESTORE_STACK
    mov rcx, QWORD PTR [rsi + 350h]
    PREPARE_STACK
    call ShopInteractionSub2
    RESTORE_STACK
    mov rax, QWORD PTR [ASMShopInteraction_jmpback]
    jmp rax
ASMShopInteractionHandler ENDP

ASMItemPriceHandler PROC
    push r8
    PREPARE_STACK
    call callPowf
    RESTORE_STACK
    pop r8
    mulss xmm0, xmm6
    mov eax, 1
    cvttss2si ecx, xmm0
    cmp ecx, eax
    cmovg eax, ecx
    PUSH_ALL
    push rax
    lea rdx, [rsp]
    mov rcx, r8
    PREPARE_STACK
    call OnItemPriceHandler
    RESTORE_STACK
    pop rax
    movq xmm0, rax
    POP_ALL
    movq rax, xmm0
    movaps xmm6, XMMWORD PTR [rsp + 20h]
    add rsp, 38h
    mov r11, QWORD PTR [ASMItemPrice_jmpback]
    jmp r11
ASMItemPriceHandler ENDP

END
