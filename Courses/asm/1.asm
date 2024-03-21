code_seg segment
ASSUME CS:CODE_SEG,DS:code_seg,ES:code_seg
org 100h

print_message macro message
    local msg, nxt
	push AX
	push DX
	
	mov DX, offset msg
	mov AH, 09h
	int 21h
	
	pop DX
	pop AX
	
	
	jmp nxt
	msg DB message, '$'
	nxt:
    endm
    
;-----------------------------------
;-----------------------------------
;-----------------------------------
start:
    
    print_message 'Input the time of your alarm (HH:MM):$'

mov AX, 4C00h
int 21h
;-----------------------------------
;-----------------------------------
;-----------------------------------
FileName DB 14, 0, 14 dup (0)
handler DW ?
RealRead DW ?
RealWrite DW ?
buffer_len EQU 2048
Bufin DB buffer_len dup (?)

CR EQU 0Dh
LF EQU 0Ah

code_seg ends
end start