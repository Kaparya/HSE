code_seg segment
ASSUME CS:code_seg,DS:code_seg,SS:code_seg
org 100h
start:
    jmp begin
print_letter macro letter
    push AX
    push DX
    
    mov DL, letter
    mov AH, 02h
    int 21h
    
    pop DX
    pop AX
endm

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

new_line macro 
    push DX
    push AX
    
    mov AH, 02h
    mov DL, CR
    int 21h
    
    mov DL, LF
    int 21h
    
    pop AX
    pop DX
endm

print_byte macro 
local cont 

    and DL, 0Fh
    add DL, 30h
    cmp DL, 39h
    jle cont
    add DL, 07h
    cont:
    int 21h

endm

print_tetrade macro 
local cont, cont1

    pop DX
    push AX
    mov AH, 02h
    shr DL, 4
    print_byte
           
    mov DL, BL
    print_byte
    
    pop AX
    
endm


get_vector macro vector, old_vector
    
    push BX
    push ES
    cli
    mov AX, 35&vector
    int 21h
    mov word ptr old_vector, BX
    mov word ptr old_vector + 2, ES
    sti
    pop ES
    pop BX
    
endm

set_vector macro vector, isr
    mov DX, offset isr
    mov AX, 25&vector
    int 21h
endm

recovery_vector macro vector, old_vector

    push DS
    lds DX, CS:old_vector
    mov AX, 25&vector
    int 21h
    pop DS
endm

new_1Ch proc far
    inc CS:counter
    cmp CS:counter, 18
    jne out_
    
    inc CS:counter1
    mov BL, CS:counter1
    xor BH, BH
    push BX
    print_tetrade
    new_line
    
    mov CS:counter, 0
    out_:
    jmp dword ptr CS:[old_1Ch] 
    counter DB 0
    counter1 DB 0
new_1Ch endp

begin:
    get_vector 1Ch, old_1Ch
    set_vector 1Ch, new_1Ch
    
    mov DI, 0
    mov CX, 100h
    c:
        xor AX, AX
    go: 
        inc AX
        cmp AX, 0
        jne go
        loop c
    recovery_vector 1Ch, old_1Ch
    
    print_message 'END TIME'    
    mov AX, 4C00h
    int 21h
    
old_1Ch DD ?
CR EQU 0Dh
LF EQU 0Ah

code_seg ends
    end start