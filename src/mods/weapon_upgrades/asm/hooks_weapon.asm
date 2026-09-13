; ==============================================================================
; cubeforge-weapon-upgrades - MASM x64 Hooks
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

EXTERN SetWeaponPreview:PROC
EXTERN UpgradeWeapon:PROC
EXTERN ASMWeaponUpgradePreview_jmpback:QWORD

PUBLIC ASMWeaponUpgradePreview
PUBLIC ASMOnWeaponUpgrade

ASMWeaponUpgradePreview PROC
    PUSH_ALL
    mov rdx, rbx
    lea rcx, [rbp + 100h]
    PREPARE_STACK
    call SetWeaponPreview
    RESTORE_STACK
    POP_ALL
    mov rax, QWORD PTR [ASMWeaponUpgradePreview_jmpback]
    jmp rax
ASMWeaponUpgradePreview ENDP

ASMOnWeaponUpgrade PROC
    PUSH_ALL
    PREPARE_STACK
    call UpgradeWeapon
    RESTORE_STACK
    POP_ALL
    ret
ASMOnWeaponUpgrade ENDP

END
