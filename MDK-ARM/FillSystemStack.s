				AREA ||.text.FillSystemStack||, CODE 
				IMPORT ||STACK$$Base|| 
				IMPORT ||STACK$$Limit|| 
				EXPORT FillSystemStack 
FillSystemStack PROC
				LDR R0, =||STACK$$Base|| 
				MOV R1, SP 
				MOV R2, #0xCDCDCDCD 
				MOV R3, #0xABABABAB 
				STR R3, [ R0 ] 
				B Loop_Check 
Loop 			STR R2, [ R0 ]
Loop_Check 		ADDS R0, R0, #0x04 
				CMP R0, R1 
				BNE Loop 
				BX LR 
				ENDP
				ALIGN 
				END