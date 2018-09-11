; Print.s
; Student names: change this to your names or look very silly
; Last modification date: change this to the last modification date or look very silly
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString

    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB


;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutDec PROC
    EXPORT   LCD_OutDec
;put your Lab 7 solution here
LinkOffset EQU 4

	;recursive implementation
	;if input is greater than 10, div by 10, call LCD_OutDec w new num
	MOV R2, #10			;set R1 remainder, R2 mod
	UDIV R1, R0, R2 	;R1=R0/10
	MLS R2, R2, R1, R0

	MOV R0, R2			;Store remainder/linkage/frame
	ADD SP, #-8			;manually store stuff in
	STR R0, [SP]
	STR LR, [SP, #LinkOffset]

	CMP R1, #0			;if((R0)<10)
	BEQ base	 		;{break;}
	
	MOV R0, R1
	BL LCD_OutDec		;else{recurse;}

base
	LDR R0, [SP]
	ADD R0, #0x30		;print digit
		PUSH{R0-R3}
	BL ST7735_OutChar
	POP{R0-R3}
	LDR LR, [SP, #LinkOffset]
	ADD SP, #8
	BX LR
    ENDP
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix PROC
         EXPORT   LCD_OutFix
ten EQU 0
hun EQU 4
thous EQU 8

	SUB SP, #16
	MOV R12, SP
	MOV R1, #10
	STR R1, [SP]
	MOV R1, #100
	STR R1, [SP, #4]
	MOV R1, #1000
	STR R1, [SP, #8]
	
	MOV R1, #9999	  ;checks if within range
	CMP R0, R1 ;checks if it is too big
	BHI outrange
	CMP R0, #0 ;check if neg
	BLO outrange

	LDR R3, [R12, #thous] ; thousands digit
	UDIV R1, R0, R3
	PUSH {R2, LR}
	PUSH{R0-R3}
	MOV R0, R1
	ADD R0, #0x30
	BL ST7735_OutChar
	MOV R0, #0x2E			;decimal
	BL ST7735_OutChar
	POP{R0-R3}
	MUL R1, R3
	SUB R0, R1
	LDR R3, [R12, #hun]		;hundreds digit
	UDIV R1, R0, R3
	PUSH{R0-R3}
	MOV R0, R1
	ADD R0, #0x30
	BL ST7735_OutChar
	POP{R0-R3}
	MUL R1, R3
	SUB R0, R1
	LDR R3, [R12]			;tens digit
	UDIV R1, R0, R3
	PUSH{R0-R3}
	MOV R0, R1
	ADD R0, #0x30
	BL ST7735_OutChar
	POP{R0-R3}
	MUL R1, R3
	SUB R0, R1				;ones digit
	PUSH{R0-R3}
	ADD R0, #0x30
	BL ST7735_OutChar
	POP{R0-R3}
	POP {R2, LR}
	ADD SP, #16
	BX   LR
outrange
	PUSH{R0, LR}
	MOV R0, #0x2A			;print '*.***'
	PUSH{R0-R3}
	BL ST7735_OutChar
	POP{R0-R3}
	MOV R0, #0x2E
	PUSH{R0-R3}
	BL ST7735_OutChar
	POP{R0-R3}
	MOV R0, #0x2A
	PUSH{R0-R3}
	BL ST7735_OutChar
	POP{R0-R3}
	MOV R0, #0x2A
	PUSH{R0-R3}
	BL ST7735_OutChar
	POP{R0-R3}
	MOV R0, #0x2A
	PUSH{R0-R3}
	BL ST7735_OutChar
	POP{R0-R3}
OFdun
	POP{R0, LR}
	ADD SP, #16
         BX LR
         ENDP

         ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

    ALIGN                           ; make sure the end of this section is aligned
    END                             ; end of file
