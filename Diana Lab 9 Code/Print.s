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
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix
		
Number					EQU 0
Number_MOD				EQU 4
Multipler				EQU 8
StackPT					EQU 12
	
	
Char1 EQU 0
Char2 EQU 4
Char3 EQU 8
Char4 EQU 12

    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutDec
;; --UUU-- Complete this (copy from Lab7-8)
		MOV R2, #10					;Multipler is 10
		UDIV R1, R0, R2				;R1 has Input/10
		SUB SP, SP, #16				;PUSH {R0, R1, R2, LR}
		STR R0, [SP, #Number]
		STR R1, [SP, #Number_MOD]
		STR R2, [SP, #Multipler]
		STR LR, [SP, #StackPT]
		CMP R1, #0
		BEQ Done					;Finished recursing if 0/10
		UDIV R0, R2
		BL LCD_OutDec
Done	LDR R0, [SP, #Number]
		LDR R1, [SP, #Number_MOD]
		MUL R1, R2					
		SUB R0, R1					;R0 = R0%10 || R0 = R0 - R1*10
		ADD R0, R0, #0x30			;Convert number to char
		BL ST7735_OutChar
		LDR R2, [SP, #Multipler]
		LDR LR, [SP, #StackPT]
		ADD SP, SP, #16							;POP {R0, R1, R2, LR}
		
    BX LR
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
LCD_OutFix
;; --UUU-- Complete this (copy from Lab7-8)
		PUSH {R0, LR}
		BL 	ST7735_OutChar
		POP {R0, LR}

     BX LR
;* * * * * * * * End of LCD_OutFix * * * * * * * *

    ALIGN                           ; make sure the end of this section is aligned
    END                             ; end of file
