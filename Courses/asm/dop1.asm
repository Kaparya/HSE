;REGISTER OUTPUT

code_seg segment
ASSUME CS:CODE_SEG,DS:code_seg,ES:code_seg
org 100h
start:
jmp begin

;===========================================

print_byte macro 
local print
    
    and DL, 0Fh

    add DL, 30h
    cmp DL, 39h
    jbe print
    
    add DL, 07h
    
    print:
        mov AH, 02h
        int 21h
    
endm

print_word macro
    
    push DX
    
    shr DL, 4
    print_byte
    
    pop DX
    print_byte
    
endm

print_reg macro
    
    push AX
    push DX
    
    
    push DX
    
    mov DL, DH
    print_word
    
    pop DX
    print_word
    
    
    pop DX
    pop AX
    
endm

print_message macro message
local msg, out_
    
    push AX
    push DX

    mov AH, 09h
    mov DX, offset msg
    int 21h

    jmp out_
    
    msg DB message, '$'
    
    out_:
    
    pop DX
    pop AX
endm

;===========================================

begin:
    mov DX, 0F09Fh
    
    print_message 'In DX: '
    print_reg
    
    int 20h

;===========================================

code_seg ends
end start