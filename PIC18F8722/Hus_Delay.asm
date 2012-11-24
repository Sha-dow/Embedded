#include "p18cxxx.inc"

	EXTERN DelayCounter1
D100USCODE	CODE


Hus_Delay  FUSTART
        movlw   0xff
        stkadj    1                    
        movf    PLUSW1,0
        movlb   DelayCounter1
        movwf   DelayCounter1
        movlw   0x1a
	bra     $+2
	bra     $+2
        bra     D100_1

D100x
        movlw   0x20
D100_1
        movwf   INDF1
        decfsz  INDF1,1
        bra     $-2
        decfsz  DelayCounter1,1
        bra     D100x
        bra     $+2
        FUEND
        return

        GLOBAL  Hus_Delay

        END