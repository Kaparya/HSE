code_seg segment
ASSUME CS:CODE_SEG,DS:code_seg,ES:code_seg
org 100h
start:
jmp begin
;===========================================================================
;===========================================================================
;===========================================================================
    old_01h DD ?
;===========================================================================
new_01h proc far

    jmp     dword ptr CS:[old_01h]  ; В системный обработчик без возврата

endp
;===========================================================================
begin:
	
	push ES
	mov AX, 3501h
	int 21h
	pop ES
	
	cmp BX, offset CS:new_01h
	jne install
	
	jmp uninstall
	
	install:
	
	mov AX, 3501h
	int 21h
        mov word ptr old_01h,BX  ; ES:BX - вектор
        mov word ptr old_01h+2,ES
	
	mov AX, 2501h
	mov DX, offset new_01h
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
    
    lds DX, old_01h   ; Эта команда эквивалентна следующим двум
                                    ; mov DX, word ptr old_2Fh
                                    ; mov DS, word ptr old_2Fh+2
    mov AX,2501h
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