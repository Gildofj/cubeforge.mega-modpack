; ==============================================================================
; cubeforge-megamod - MASM x64 Native Hooks (Agregador de Todos os Módulos)
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

; ------------------------------------------------------------------------------
; Funções Externas C/C++
; ------------------------------------------------------------------------------
EXTERN OnChestInteraction:PROC
EXTERN OnCreatureDeath:PROC
EXTERN OnTalkToCreature:PROC
EXTERN GetDropRegion:PROC
EXTERN callPowf:PROC
EXTERN OnItemPriceHandler:PROC
EXTERN OnPrintRGBA:PROC
EXTERN OnGetEffectiveItemRarity:PROC
EXTERN OnShopInteractionHandler:PROC
EXTERN ShopInteractionSub1:PROC
EXTERN ShopInteractionSub2:PROC
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
EXTERN OnGetItemDescription:PROC
EXTERN OnGetItemName:PROC
EXTERN SetWeaponPreview:PROC
EXTERN UpgradeWeapon:PROC
EXTERN GetRegionType:PROC
EXTERN GetBuildingType:PROC

; ------------------------------------------------------------------------------
; Variáveis Globais Externas
; ------------------------------------------------------------------------------
EXTERN ASMIsChestOpen_jmpback:QWORD
EXTERN ASMIsChestOpen_bail:QWORD
EXTERN ASMChestInteractionHandler_jmpback:QWORD
EXTERN ASMOnCreatureDeath_jmpback:QWORD
EXTERN ASMOnTalkToCreature_jmpback:QWORD
EXTERN ASMOnTalkToCreature_jmpback2:QWORD
EXTERN ASMOnTalkToCreature_bail:QWORD
EXTERN ASMPatchItemDrop_jmpback:QWORD
EXTERN ASMItemPrice_jmpback:QWORD
EXTERN ASMIncreaseLore_jmpback:QWORD
EXTERN ASMShopInteraction_jmpback:QWORD
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
EXTERN ASMOnGetItemDescription_jmpback:QWORD
EXTERN ASMOnGetItemDescription_jmpback_2:QWORD
EXTERN ASMOnGetItemDescription_bail:QWORD
EXTERN ASMOnGetItemName_jmpback:QWORD
EXTERN ASMOnGetItemName_bail:QWORD
EXTERN ASMWeaponUpgradePreview_jmpback:QWORD

; ------------------------------------------------------------------------------
; Símbolos Públicos
; ------------------------------------------------------------------------------
PUBLIC ASMIsChestOpen
PUBLIC ASMChestInteractionHandler
PUBLIC ASMOnCreatureDeath
PUBLIC ASMOnTalkToCreature
PUBLIC ASMPatchItemDrop
PUBLIC ASMItemPriceHandler
PUBLIC ASMIncreaseLore
PUBLIC ASMEffectiveItemRarity
PUBLIC ASMShopInteractionHandler
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
PUBLIC ASMOnGetItemDescription
PUBLIC ASMOnGetItemName
PUBLIC ASMWeaponUpgradePreview
PUBLIC ASMOnWeaponUpgrade
PUBLIC ASMUpdateGetBiomeType
PUBLIC ASMBuildingTypeOverride

; ------------------------------------------------------------------------------
; Implementações
; ------------------------------------------------------------------------------

; --- Chest Hooks ---
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

; --- Creature Death Hook ---
ASMOnCreatureDeath PROC
    PUSH_ALL
    mov rdx, r15
    mov rcx, r13
    PREPARE_STACK
    call OnCreatureDeath
    RESTORE_STACK
    POP_ALL
    xor r15d, r15d
    mov QWORD PTR [r13 + 180h], r15
    mov dword ptr [rbp - 41h], 3F800000h
    mov rax, QWORD PTR [ASMOnCreatureDeath_jmpback]
    jmp rax
ASMOnCreatureDeath ENDP

; --- Creature Talk Hook ---
ASMOnTalkToCreature PROC
    PUSH_ALL
    mov rdx, rdi
    mov rcx, rsi
    PREPARE_STACK
    call OnTalkToCreature
    RESTORE_STACK
    cmp rax, 1
    je Label_TalkBail
    POP_ALL
    cmp al, 2
    jbe Label_TalkJmp2
    cmp cl, 9Ch
    jz Label_TalkJmp2
    mov rax, QWORD PTR [ASMOnTalkToCreature_jmpback]
    jmp rax
Label_TalkJmp2:
    mov rax, QWORD PTR [ASMOnTalkToCreature_jmpback2]
    jmp rax
Label_TalkBail:
    POP_ALL
    mov rax, QWORD PTR [ASMOnTalkToCreature_bail]
    jmp rax
ASMOnTalkToCreature ENDP

; --- Item Drop Patch Hook ---
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

; --- Item Price Handler Hook ---
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

; --- Increase Lore Hook ---
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

; --- Region Lock Rarity Hook ---
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

; --- Shop Interaction Hook ---
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

; --- Player Updates Hooks ---
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

; --- Quest Mod Hooks ---
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

; --- Weapon Upgrade Mod Hooks ---
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

; --- World Gen Mod Hooks ---
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
