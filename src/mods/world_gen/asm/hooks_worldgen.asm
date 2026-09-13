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
    push r15
    PUSH_ALL
    mov rcx, rdx
    mov rdx, r8
    PREPARE_STACK
    call GetRegionType
    RESTORE_STACK
    mov QWORD PTR [rsp], rax
    POP_ALL
    mov rax, r15
    pop r15
    ret
ASMUpdateGetBiomeType ENDP

ASMBuildingTypeOverride PROC
    sub rsp, 110h
    movups XMMWORD PTR [rsp + 10h], xmm0
    movups XMMWORD PTR [rsp + 20h], xmm1
    movups XMMWORD PTR [rsp + 30h], xmm2
    movups XMMWORD PTR [rsp + 40h], xmm3
    movups XMMWORD PTR [rsp + 50h], xmm4
    movups XMMWORD PTR [rsp + 60h], xmm5
    movups XMMWORD PTR [rsp + 70h], xmm6
    movups XMMWORD PTR [rsp + 80h], xmm7
    movups XMMWORD PTR [rsp + 90h], xmm8
    movups XMMWORD PTR [rsp + 0A0h], xmm9
    movups XMMWORD PTR [rsp + 0B0h], xmm10
    movups XMMWORD PTR [rsp + 0C0h], xmm11
    movups XMMWORD PTR [rsp + 0D0h], xmm12
    movups XMMWORD PTR [rsp + 0E0h], xmm13
    movups XMMWORD PTR [rsp + 0F0h], xmm14
    movups XMMWORD PTR [rsp + 100h], xmm15
    PREPARE_STACK
    call GetBuildingType
    RESTORE_STACK
    movups xmm0, XMMWORD PTR [rsp + 10h]
    movups xmm1, XMMWORD PTR [rsp + 20h]
    movups xmm2, XMMWORD PTR [rsp + 30h]
    movups xmm3, XMMWORD PTR [rsp + 40h]
    movups xmm4, XMMWORD PTR [rsp + 50h]
    movups xmm5, XMMWORD PTR [rsp + 60h]
    movups xmm6, XMMWORD PTR [rsp + 70h]
    movups xmm7, XMMWORD PTR [rsp + 80h]
    movups xmm8, XMMWORD PTR [rsp + 90h]
    movups xmm9, XMMWORD PTR [rsp + 0A0h]
    movups xmm10, XMMWORD PTR [rsp + 0B0h]
    movups xmm11, XMMWORD PTR [rsp + 0C0h]
    movups xmm12, XMMWORD PTR [rsp + 0D0h]
    movups xmm13, XMMWORD PTR [rsp + 0E0h]
    movups xmm14, XMMWORD PTR [rsp + 0F0h]
    movups xmm15, XMMWORD PTR [rsp + 100h]
    add rsp, 110h
    ret
ASMBuildingTypeOverride ENDP

END
