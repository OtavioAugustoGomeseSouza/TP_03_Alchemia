    MAC CLEAN_START
    SEI
    CLD
    LDX #$FF
    TXS
    LDA #0
.CleanLoop:
    STA 0,X
    DEX
    BNE .CleanLoop
    ENDM
