; ==============================================================================
; cubeforge-player-updates - MASM x64 Hooks
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

EXTERN GetCubeClassNameWide:PROC
EXTERN GetCubeClassName:PROC
EXTERN OnCharacterWidgetClassButtonIncrease:PROC
EXTERN OnCharacterWidgetClassButtonDecrease:PROC
EXTERN GenerateStarterGear:PROC
EXTERN OnCanEquipItem:PROC
EXTERN OnCreatureManaModification:PROC
EXTERN OnExecuteAbility:PROC
EXTERN OnGetHairColorDisplay:PROC
EXTERN OnGetHairColorText:PROC
EXTERN OnGetHairCutText:PROC
EXTERN OnGetFaceText:PROC
EXTERN OnGetGenderText:PROC
EXTERN OnGenerateCreature:PROC
EXTERN OnGetRaceName:PROC
EXTERN OnGetRaceName2:PROC
EXTERN OnShiftAbilityID:PROC
EXTERN OnGetUltimateAbilityCooldown:PROC
EXTERN OnGetUltimateAbilityID:PROC
EXTERN GetTreasureDropClass:PROC

EXTERN ASMSetMenuClassName_retn:QWORD
EXTERN ASMSetClassName_1_ret:QWORD
EXTERN ASMSetClassName_2_ret:QWORD
EXTERN OverwriteClassIdIncrease_ret:QWORD
EXTERN OverwriteClassIdDecrease_ret:QWORD
EXTERN AddCreatureStarterWeapons_ret:QWORD

EXTERN ASMOnCanEquipItem_jmpback:QWORD
EXTERN ASMOnCanEquipItem_jmpback_2:QWORD
EXTERN ASMOnCanEquipItem_bail:QWORD

EXTERN ASMOnManaHandler_jmpback:QWORD

EXTERN ASMOnExecuteAbility_jmpback:QWORD
EXTERN ASMOnExecuteAbility_jmpback_2:QWORD

EXTERN ASMOnDrawColorPicker_jmpback:QWORD
EXTERN ASMOnDrawColorPicker_bail:QWORD
EXTERN ASMOnDrawHairColor_jmpback:QWORD
EXTERN ASMOnDrawHairCut_jmpback:QWORD
EXTERN ASMOnDrawFace_jmpback:QWORD
EXTERN ASMOnDrawGender_jmpback:QWORD
EXTERN ASMOnGenerateCreature_jmpback:QWORD
EXTERN ASMOnGetRaceName_jmpback:QWORD
EXTERN ASMOnGetRaceName2_jmpback:QWORD

EXTERN ASMOnGetShiftAbilityID_jmpback:QWORD
EXTERN ASMOnGetShiftAbilityID_jmpback_2:QWORD

EXTERN ASMTreasureFixHandler_jmpback:QWORD
EXTERN ASMTreasureFixHandler_ret_1:QWORD
EXTERN ASMTreasureFixHandler_ret_2:QWORD

PUBLIC ASMSetMenuClassName
PUBLIC ASMSetClassName_1
PUBLIC ASMSetClassName_2
PUBLIC ASMMenuClassIdIncrease
PUBLIC ASMMenuClassIdDecrease
PUBLIC ASMAddCreatureStarterWeapons
PUBLIC ASMCharacterStyleWidgetUpdateCreature
PUBLIC ASMOnCanEquipItem
PUBLIC ASMOnManaHandler
PUBLIC ASMOnExecuteAbility
PUBLIC ASMOnDrawColorPicker
PUBLIC ASMOnDrawHairColor
PUBLIC ASMOnDrawHairCut
PUBLIC ASMOnDrawFace
PUBLIC ASMOnDrawGender
PUBLIC ASMOnGenerateCreature
PUBLIC ASMOnGetRaceName
PUBLIC ASMOnGetRaceName2
PUBLIC ASMOnGetShiftAbilityID
PUBLIC ASMOnGetUltimateAbilityCooldown
PUBLIC ASMOnGetUltimateAbilityID
PUBLIC ASMTreasureFixHandler

; --- Character Creation Hooks ---
ASMSetMenuClassName PROC
    sub ecx, 1
    push rax
    PREPARE_STACK
    call GetCubeClassNameWide
    RESTORE_STACK
    mov rdx, rax
    pop rax
    mov r11, QWORD PTR [ASMSetMenuClassName_retn]
    jmp r11
ASMSetMenuClassName ENDP

ASMSetClassName_1 PROC
    sub ecx, 1
    push rax
    PREPARE_STACK
    call GetCubeClassName
    RESTORE_STACK
    mov rdx, rax
    pop rax
    mov r11, QWORD PTR [ASMSetClassName_1_ret]
    jmp r11
ASMSetClassName_1 ENDP

ASMSetClassName_2 PROC
    sub ecx, 1
    push rax
    PREPARE_STACK
    call GetCubeClassName
    RESTORE_STACK
    mov rdx, rax
    pop rax
    mov r11, QWORD PTR [ASMSetClassName_2_ret]
    jmp r11
ASMSetClassName_2 ENDP

ASMMenuClassIdIncrease PROC
    PUSH_ALL
    mov rcx, rbx
    PREPARE_STACK
    call OnCharacterWidgetClassButtonIncrease
    RESTORE_STACK
    POP_ALL
    xor edx, edx
    mov rcx, rbx
    mov rax, QWORD PTR [OverwriteClassIdIncrease_ret]
    jmp rax
ASMMenuClassIdIncrease ENDP

ASMMenuClassIdDecrease PROC
    PUSH_ALL
    mov rcx, rbx
    PREPARE_STACK
    call OnCharacterWidgetClassButtonDecrease
    RESTORE_STACK
    POP_ALL
    xor edx, edx
    mov rcx, rbx
    mov rax, QWORD PTR [OverwriteClassIdDecrease_ret]
    jmp rax
ASMMenuClassIdDecrease ENDP

ASMAddCreatureStarterWeapons PROC
    PUSH_ALL
    mov rcx, QWORD PTR [rdi + 1E8h]
    PREPARE_STACK
    call GenerateStarterGear
    RESTORE_STACK
    POP_ALL
    mov rcx, QWORD PTR [rdi + 1E8h]
    mov rcx, QWORD PTR [rcx + 8]
    mov rax, QWORD PTR [AddCreatureStarterWeapons_ret]
    jmp rax
ASMAddCreatureStarterWeapons ENDP

ASMCharacterStyleWidgetUpdateCreature PROC
    ret
ASMCharacterStyleWidgetUpdateCreature ENDP

; --- Equip Item Hook ---
ASMOnCanEquipItem PROC
    PUSH_ALL
    PREPARE_STACK
    call OnCanEquipItem
    RESTORE_STACK
    cmp rax, 0
    jne Label_CanEquip_Bail
    POP_ALL
    movzx eax, byte ptr [rcx]
    mov r9, rcx
    cmp al, 2
    je Label_CanEquip_Jmp2
    mov r11, QWORD PTR [ASMOnCanEquipItem_jmpback]
    jmp r11
Label_CanEquip_Jmp2:
    mov r11, QWORD PTR [ASMOnCanEquipItem_jmpback_2]
    jmp r11
Label_CanEquip_Bail:
    POP_ALL
    mov r11, QWORD PTR [ASMOnCanEquipItem_bail]
    jmp r11
ASMOnCanEquipItem ENDP

; --- Mana Handler Hook ---
ASMOnManaHandler PROC
    movaps xmm1, xmm10
    mulss xmm1, xmm12
    movss xmm0, DWORD PTR [r13 + 188h]
    PUSH_ALL
    PREPARE_STACK
    mov rcx, r13
    call OnCreatureManaModification
    RESTORE_STACK
    cmp rax, 0
    je Label_ManaLoss
    cmp rax, 1
    je Label_ManaGain
    POP_ALL
    mov r11, QWORD PTR [ASMOnManaHandler_jmpback]
    jmp r11
Label_ManaLoss:
    POP_ALL
    subss xmm0, xmm1
    mov r11, QWORD PTR [ASMOnManaHandler_jmpback]
    jmp r11
Label_ManaGain:
    POP_ALL
    addss xmm0, xmm1
    mov r11, QWORD PTR [ASMOnManaHandler_jmpback]
    jmp r11
ASMOnManaHandler ENDP

; --- Execute Ability Hook ---
ASMOnExecuteAbility PROC
    mov rax, rsp
    sub rsp, 110h
    movaps XMMWORD PTR [rax - 10h], xmm0
    movaps XMMWORD PTR [rax - 20h], xmm1
    movaps XMMWORD PTR [rax - 30h], xmm2
    movaps XMMWORD PTR [rax - 40h], xmm3
    movaps XMMWORD PTR [rax - 50h], xmm4
    movaps XMMWORD PTR [rax - 60h], xmm5
    movaps XMMWORD PTR [rax - 70h], xmm6
    movaps XMMWORD PTR [rax - 80h], xmm7
    movaps XMMWORD PTR [rax - 90h], xmm8
    movaps XMMWORD PTR [rax - 0A0h], xmm9
    movaps XMMWORD PTR [rax - 0B0h], xmm10
    movaps XMMWORD PTR [rax - 0C0h], xmm11
    movaps XMMWORD PTR [rax - 0D0h], xmm12
    movaps XMMWORD PTR [rax - 0E0h], xmm13
    movaps XMMWORD PTR [rax - 0F0h], xmm14
    movaps XMMWORD PTR [rax - 100h], xmm15
    PUSH_ALL
    mov rdx, r13
    mov rcx, r15
    PREPARE_STACK
    call OnExecuteAbility
    RESTORE_STACK
    POP_ALL
    mov rax, rsp
    add rax, 110h
    movaps xmm0, XMMWORD PTR [rax - 10h]
    movaps xmm1, XMMWORD PTR [rax - 20h]
    movaps xmm2, XMMWORD PTR [rax - 30h]
    movaps xmm3, XMMWORD PTR [rax - 40h]
    movaps xmm4, XMMWORD PTR [rax - 50h]
    movaps xmm5, XMMWORD PTR [rax - 60h]
    movaps xmm6, XMMWORD PTR [rax - 70h]
    movaps xmm7, XMMWORD PTR [rax - 80h]
    movaps xmm8, XMMWORD PTR [rax - 90h]
    movaps xmm9, XMMWORD PTR [rax - 0A0h]
    movaps xmm10, XMMWORD PTR [rax - 0B0h]
    movaps xmm11, XMMWORD PTR [rax - 0C0h]
    movaps xmm12, XMMWORD PTR [rax - 0D0h]
    movaps xmm13, XMMWORD PTR [rax - 0E0h]
    movaps xmm14, XMMWORD PTR [rax - 0F0h]
    movaps xmm15, XMMWORD PTR [rax - 100h]
    add rsp, 110h
    xorps xmm15, xmm15
    cmp byte ptr [r13 + 68h], 85h
    jnz @F
    movss xmm0, DWORD PTR [r13 + 44h]
    mov r11, QWORD PTR [ASMOnExecuteAbility_jmpback]
    jmp r11
@@:
    mov r11, QWORD PTR [ASMOnExecuteAbility_jmpback_2]
    jmp r11
ASMOnExecuteAbility ENDP

; --- Creature Appearance Hooks ---
ASMOnDrawColorPicker PROC
    mov rcx, rdi
    PREPARE_STACK
    call OnGetHairColorDisplay
    RESTORE_STACK
    xorps xmm0, xmm0
    movdqu XMMWORD PTR [rbp + 58h], xmm0
    mov QWORD PTR [rbp + 68h], rsi
    cmp rax, 1
    jne @F
    mov r11, QWORD PTR [ASMOnDrawColorPicker_jmpback]
    jmp r11
@@:
    mov r11, QWORD PTR [ASMOnDrawColorPicker_bail]
    jmp r11
ASMOnDrawColorPicker ENDP

ASMOnDrawHairColor PROC
    lea rcx, [rsp + 50h]
    mov rdx, rdi
    PREPARE_STACK
    call OnGetHairColorText
    RESTORE_STACK
    mov rax, QWORD PTR [ASMOnDrawHairColor_jmpback]
    jmp rax
ASMOnDrawHairColor ENDP

ASMOnDrawHairCut PROC
    lea rcx, [rsp + 50h]
    mov rdx, rdi
    PREPARE_STACK
    call OnGetHairCutText
    RESTORE_STACK
    mov rax, QWORD PTR [ASMOnDrawHairCut_jmpback]
    jmp rax
ASMOnDrawHairCut ENDP

ASMOnDrawFace PROC
    lea rcx, [rsp + 50h]
    mov rdx, rdi
    PREPARE_STACK
    call OnGetFaceText
    RESTORE_STACK
    mov rax, QWORD PTR [ASMOnDrawFace_jmpback]
    jmp rax
ASMOnDrawFace ENDP

ASMOnDrawGender PROC
    lea rcx, [rsp + 50h]
    mov rdx, rdi
    PREPARE_STACK
    call OnGetGenderText
    RESTORE_STACK
    mov rax, QWORD PTR [ASMOnDrawGender_jmpback]
    jmp rax
ASMOnDrawGender ENDP

ASMOnGenerateCreature PROC
    PUSH_ALL
    mov rcx, r12
    mov rdx, r8
    mov r8, rbx
    PREPARE_STACK
    call OnGenerateCreature
    RESTORE_STACK
    POP_ALL
    mov rax, QWORD PTR [ASMOnGenerateCreature_jmpback]
    jmp rax
ASMOnGenerateCreature ENDP

ASMOnGetRaceName PROC
    lea rcx, [rsp + 50h]
    mov rdx, rax
    PREPARE_STACK
    call OnGetRaceName
    RESTORE_STACK
    mov r11, QWORD PTR [ASMOnGetRaceName_jmpback]
    jmp r11
ASMOnGetRaceName ENDP

ASMOnGetRaceName2 PROC
    lea rcx, [rsp + 50h]
    mov rdx, rax
    PREPARE_STACK
    call OnGetRaceName2
    RESTORE_STACK
    mov r11, QWORD PTR [ASMOnGetRaceName2_jmpback]
    jmp r11
ASMOnGetRaceName2 ENDP

; --- Shift & Ultimate Hooks ---
ASMOnGetShiftAbilityID PROC
    mov rax, QWORD PTR [r13 + 8]
    mov rcx, QWORD PTR [rax + 448h]
    PUSH_ALL
    mov rdx, rcx
    mov rcx, r13
    PREPARE_STACK
    call OnShiftAbilityID
    RESTORE_STACK
    POP_ALL
    cmp byte ptr [rcx + 14Ch], 4
    jne @F
    mov rax, QWORD PTR [ASMOnGetShiftAbilityID_jmpback]
    jmp rax
@@:
    mov rax, QWORD PTR [ASMOnGetShiftAbilityID_jmpback_2]
    jmp rax
ASMOnGetShiftAbilityID ENDP

ASMOnGetUltimateAbilityCooldown PROC
    PREPARE_STACK
    call OnGetUltimateAbilityCooldown
    RESTORE_STACK
    ret
ASMOnGetUltimateAbilityCooldown ENDP

ASMOnGetUltimateAbilityID PROC
    PREPARE_STACK
    call OnGetUltimateAbilityID
    RESTORE_STACK
    ret
ASMOnGetUltimateAbilityID ENDP

; --- Treasure Fix Hook ---
ASMTreasureFixHandler PROC
    PUSH_ALL
    mov rcx, r12
    PREPARE_STACK
    call GetTreasureDropClass
    RESTORE_STACK
    mov QWORD PTR [rsp + 60h], rax
    POP_ALL
    sub ecx, 1
    jz Label_TF_Ret1
    sub ecx, 1
    jz Label_TF_Ret2
    mov rax, QWORD PTR [ASMTreasureFixHandler_jmpback]
    jmp rax
Label_TF_Ret1:
    mov rax, QWORD PTR [ASMTreasureFixHandler_ret_1]
    jmp rax
Label_TF_Ret2:
    mov rax, QWORD PTR [ASMTreasureFixHandler_ret_2]
    jmp rax
ASMTreasureFixHandler ENDP

END
