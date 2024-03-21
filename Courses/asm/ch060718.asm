code_seg segment
ASSUME CS:CODE_SEG,DS:code_seg,ES:code_seg
org 100h
start:
jmp begin
;===========================================================================
;===========================================================================
;===========================================================================
    old_06h DD ?
    old_07h DD ?
    old_18h DD ?
;===========================================================================
new_06h proc far

    jmp     dword ptr CS:[old_06h]  ; В системный обработчик без возврата

endp

new_07h proc far

    jmp     dword ptr CS:[old_07h]  ; В системный обработчик без возврата

endp

new_18h proc far

    jmp     dword ptr CS:[old_18h]  ; В системный обработчик без возврата

endp
;===========================================================================
begin:
	
	push ES
	mov AX, 3506h
	int 21h
	pop ES
	
	cmp BX, offset CS:new_06h
	jne install
	
	jmp uninstall
	
	install:
	
	mov AX, 3506h
	int 21h
        mov word ptr old_06h,BX  ; ES:BX - вектор
        mov word ptr old_06h+2,ES
	
	mov AX, 2506h
	mov DX, offset new_06h
	int 21h
	
	mov AX, 3507h
	int 21h
        mov word ptr old_07h,BX  ; ES:BX - вектор
        mov word ptr old_07h+2,ES
	
	mov AX, 2507h
	mov DX, offset new_07h
	int 21h
	
	mov AX, 3518h
	int 21h
        mov word ptr old_18h,BX  ; ES:BX - вектор
        mov word ptr old_18h+2,ES
	
	mov AX, 2518h
	mov DX, offset new_18h
	int 21h
	
;---------------------------------------------------------------------------

	mov AH, 09h
        mov DX, offset CS:msg111 ; Сообщение об установке
        int 21h
	
        mov DX,offset begin ; оставить программу ...
        int 27h             ; ... резидентной и выйти
; ------------------ Выгрузка ----------------------------------------------
uninstall:
    push DS
    
    lds DX, old_06h   ; Эта команда эквивалентна следующим двум
                                    ; mov DX, word ptr old_2Fh
                                    ; mov DS, word ptr old_2Fh+2
    mov AX,2506h
    int 21h
    
    pop DS
    
    push DS
    
    lds DX, old_07h   ; Эта команда эквивалентна следующим двум
                                    ; mov DX, word ptr old_2Fh
                                    ; mov DS, word ptr old_2Fh+2
    mov AX,2507h
    int 21h
    
    pop DS
    
    push DS
    
    lds DX, old_18h   ; Эта команда эквивалентна следующим двум
                                    ; mov DX, word ptr old_2Fh
                                    ; mov DS, word ptr old_2Fh+2
    mov AX,2518h
    int 21h
    
    pop DS
    
    mov AH, 09h
    mov DX, offset CS:msg1112
    int 21h
    
    int 20h
;---------------------------------------------------------------------------
    msg111 DB 'installed', 13, 10,'$'
    msg1112 DB 'uninstalled', 13, 10,'$'
;===========================================================================
code_seg ends
end start