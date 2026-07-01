    PROCESSOR 6502
    INCLUDE "vcs.h"
    INCLUDE "macro.h"

    SEG.U RAM
    ORG $85
resultado    ds 1
vida         ds 1
i            ds 1
t0           ds 1
t2           ds 1
t1           ds 1

    SEG CODE
    ORG $F000

START:
    CLEAN_START

; vida = 100
    LDA #100
    STA vida
; resultado = 0
    LDA #0
    STA resultado
; i = 0
    LDA #0
    STA i
; i = 0
    LDA #0
    STA i
L0:
; t0 = i < 5
    LDA i
    CMP #5
    BCC _t0
_f0:
    LDA #0
    STA t0
    JMP _e0
_t0:
    LDA #1
    STA t0
_e0:
; ifFalse t0 goto L1
    LDA t0
    BNE _s1
    JMP L1
_s1:
; t2 = resultado + i
    LDA resultado
    CLC
    ADC i
    STA t2
; resultado = t2
    LDA t2
    STA resultado
; t1 = i + 1
    LDA i
    CLC
    ADC #1
    STA t1
; i = t1
    LDA t1
    STA i
; goto L0
    JMP L0
L1:

FIM_LOOP:
    JMP FIM_LOOP

    ORG $FFFC
    .word START
    .word START

; Mapa de memoria
; $85 = resultado  ; variavel final obrigatoria
; $86 = vida
; $87 = i
; $88 = t0
; $89 = t2
; $8A = t1
