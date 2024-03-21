;PSP OUTPUT

code_seg segment
ASSUME CS:CODE_SEG,DS:code_seg,ES:code_seg
org 100h
start:
jmp begin

;===========================================

print_letter macro letter
    push AX
    push DX
    
    mov AH, 02h
    mov DL, letter
    int 21h
    
    pop DX
    pop AX
endm

print_message macro message
local msg, nxt
    push AX
    push DX
    
    mov AH, 09h
    mov DX, offset msg
    int 21h
    
    jmp nxt
    
    msg DB message, '$'
    
    nxt:
    
    pop DX
    pop AX
endm

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
    push AX
    push DX

    push DX
    
    shr DL, 4
    print_byte
    
    pop DX
    print_byte
    
    pop DX
    pop AX
endm

new_line macro 
    push AX
    push DX
    
    mov AH, 02h
    mov DL, CR
    int 21h
    
    mov DL, LF
    int 21h
    
    pop DX
    pop AX
endm

;===========================================

begin:

    mov CL, 0
    mov SI, 0 ;PSP index
    
    cycleI:
        
        mov DL, CL
        cmp DL, 0Ah ; In DL - DEC number of row
        jb nxt
        
        sub DL, 0Ah
        add DL, 10h
        
        nxt:
        
        print_word
        print_message ': '
        
        push CX
        mov CL, 10h
        
        cycleJ:
            
            mov DL, ES:[SI]
            inc SI
            print_word
            print_letter ' '
            
        loop cycleJ
        
        new_line
        
        pop CX
        
        inc CL
        cmp CL, 10h
        je out_
        
        jmp cycleI
            
    out_:
    int 20h
    
;===========================================
    
LF EQU 0Ah
CR EQU 0Dh

code_seg ends
end start