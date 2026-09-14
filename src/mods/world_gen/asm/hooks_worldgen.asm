; ==============================================================================
; cubeforge-world-gen - MASM x64 Hooks
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

EXTERN GetRegionType:PROC
EXTERN GetBuildingType:PROC

PUBLIC ASMUpdateGetBiomeType
PUBLIC ASMBuildingTypeOverride

ASMUpdateGetBiomeType PROC
    sub rsp, 28h
    mov rcx, rdx
    mov rdx, r8
    call GetRegionType
    add rsp, 28h
    ret
ASMUpdateGetBiomeType ENDP

ASMBuildingTypeOverride PROC
    sub rsp, 28h
    call GetBuildingType
    add rsp, 28h
    ret
ASMBuildingTypeOverride ENDP

END
