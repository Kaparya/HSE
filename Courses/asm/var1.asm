code_seg segment
ASSUME CS:CODE_SEG,DS:code_seg,ES:code_seg
org 100h

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

check_second_name_psp macro
local cycle_, cont, second_name

    cycle_:
        mov BL, ES:[DI]
        cmp BL, 0h
        je cont
        
        cmp BL, 20h
        je second_name
        
        inc DI
        
    jmp cycle_
        
    
    second_name:
        mov BL, 0h
        mov ES:[DI], BL
        inc DI
        mov BL, ES:[DI]
        cmp BL, 20h
        je second_name
        mov SI, DI
    cont:
endm

read_file_name_psp macro
    xor BH, BH
    mov BL, ES:[80h]
    mov byte ptr [BX+81h], 0
    mov CL, ES:80h
    xor CH, CH
    cld
    mov DI, 81h
    mov AL, ' '
    repe scasb      
    dec DI
    push DI
    check_second_name_psp
    jmp openFile
endm

read_file_name_input macro
    print_message 'Type your file name: '
    mov AH, 0Ah
    mov DX, offset FileName
    int 21h
        
    xor BH, BH
    mov BL, FileName[1]
    mov FileName[BX + 2], 0
    mov DX, offset FileName + 2
    push DX
    mov DI, 0h
endm
;-----------------------------------
ReadFile macro Handler, Buffer, Counter, RealRead
local read_error, nx, m1
    
    clc
    push AX
    
    mov BX, Handler
    mov CX, Counter
    lea DX, Buffer
    mov AH, 3Fh
    int 21h
    jnc m1
    jmp read_error
    
    m1:
        mov RealRead, AX
        jmp nx
    read_error:
        new_line
        print_message 'Read_ERROR'
    nx:
        pop AX
endm
;-----------------------------------
WriteFile macro Handler, Buffer, Counter, RealWrite
local write_error, m2, m1

    clc
    push AX
    
    mov BX, Handler
    mov CX, Counter
    lea DX, Buffer
    mov AH, 40h
    int 21h
    jnc m1
    jmp write_error
    
    m1: 
        mov RealWrite, AX
        jmp m2
    write_error:
        new_line
        print_message 'Write_ERROR'
    m2:
        pop AX

endm
;-----------------------------------
;-----------------------------------
;-----------------------------------
start:
    new_line
    
    mov SI, 0h
    mov DI, 80h
    mov AL, ES:[DI]
    cmp AL, 0
    je fileNameRequest
    
    read_file_name_psp

    fileNameRequest:
        read_file_name_input
        
    openFile:
        mov AX, 3D02h
        pop DX
        int 21h
        jnc openOK
        jmp $error_input
    
openOK:
    mov handler, AX 
    mov handler1, AX
    new_line
    cmp SI, 0h
    jne openFile_output
    jmp cycle
    
    openFile_output:
    
        mov AX, 3D02h
        mov DX, SI
        int 21h
        jc $error_output
        mov handler1, AX
        jmp cycle
    
$error_output:

    new_line
    print_message 'OUTPUT FILE DOES NOT EXIST'
    new_line
    print_message 'CREATING FILE: '
    mov AH, 09h
    mov byte ptr [BX+81h], '$'
    mov DX, SI
    int 21h
    mov byte ptr [BX+81h], 0
    
    mov AX, 3C02h
    mov DX, SI
    mov CX, 0
    int 21h
    mov handler1, AX
    jmp cycle

$error_input:
    new_line
    print_message 'INPUT FILE OPEN ERROR'
    int 20h
    
;---------------------------------
mov CX, RealRead
mov SI, 0h
mov DI, 0h
c:
    dec CX
    mov Dl, Bufin[SI]
    cmp DL, 0Ah
    je center
    inc DI
    jmp c
    
    center:
        push CX
        ;find number of spaces before string
        div DI, 2
        mov CX, 40d
        sub CX, DI
        ;number of spaces in CX 
        mov DI, 39d
        c1:
            
        
        pop CX
        
;---------------------------------

cycle:
    ReadFile handler, Bufin, buffer_len, RealRead
    cmp RealRead, buffer_len
    je gowr
    jmp gohome

gowr: 
    WriteFile handler1, Bufin, buffer_len, RealWrite
    jmp cycle

gohome:
    WriteFile handler1, Bufin, RealRead, RealWrite

mov AX, 4C00h
int 21h
;-----------------------------------
;-----------------------------------
;-----------------------------------
FileName DB 14, 0, 14 dup (0)
handler DW ?
handler1 DW ?
RealRead DW ?
RealWrite DW ?
buffer_len EQU 20480
Bufin DB buffer_len dup (?)

CR EQU 0Dh
LF EQU 0Ah

code_seg ends
end start