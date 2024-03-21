;REGISTER INPUT

code_seg segment
ASSUME CS:CODE_SEG,DS:code_seg,ES:code_seg
org 100h
start:
jmp begin

print_mes macro message
local msg, nxt
    push AX
    push DX
    mov DX, offset msg
    mov AH, 09h
    int 21h
    pop DX
    pop AX
    jmp nxt
    msg DB message,'$'
    nxt:
endm

new_line macro 
	push DX
	push AX
	
	mov AH, 02h
	mov DL, 0Dh
	int 21h
	
	mov DL, 0Ah
	int 21h
	
	pop AX
	pop DX
    endm
    
    print_tetrada proc near
      add DL, 030H
      cmp DL, 03AH
      jb print1    
                   
      add DL, 07H  
      print1:       
      int 21H
      ret
    endp

    print_byte proc near
      push DX            
      shr DL, 4          
      mov AH, 02H
      call print_tetrada
      pop DX             
      and DL,0Fh
      call print_tetrada
      ret
    endp

;===========================================

begin:
    
    mov AH, 2Ch
    int 21h
    
    mov DL, DH
    call print_byte
    print_mes ':'
    mov DL, CL
    call print_byte
    print_mes ':'
    mov DL, CH
    call print_byte
    new_line
    
    int 20h
    
;===========================================

code_seg ends
end start