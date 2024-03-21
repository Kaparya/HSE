CODE_SEG SEGMENT
ASSUME CS:CODE_SEG,DS:CODE_SEG,SS:CODE_SEG
ORG 100H
START:
JMP BEGIN
CR EQU 13
LF EQU 10
;=============================macro=================================
print_letter macro letter
    push AX
    push DX
    mov DL, letter
    mov AH, 02
    int 21h
    pop DX
    pop AX
endm
;===================================================================
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
;===================================================================
new_line macro
    print_letter CR
    print_letter LF
endm
;===================================================================
get_vector macro vector, old_vector
    push BX
    push ES
    mov AX, 35&vector ; получить вектор прерывания
    int 21h
    mov word ptr old_vector, BX ; ES:BX - вектор
    mov word ptr old_vector+2, ES
    pop ES
    pop BX
endm
;===================================================================
set_vector macro vector, isr
    mov DX, offset isr ; получить смещение точки входа в новый обработчик на DX
    mov AX, 25&vector ; функция установки прерывания
    int 21h ; изменить вектор AL - номер прерыв. DS:DX - ук-ль программы обр. прер.
endm
;===================================================================
recovery_vector macro vector, old_vector
    push DS
    lds DX, CS:old_vector
    mov AX, 25&vector ; Заполнение вектора старым содержимым
    int 21h ; DS:DX - указатель программы обработки пр
    pop DS
endm
;===================================================================
;===================================================================
new_1Ch proc far
    push CX
    
    inc CS:time
    cmp CS:time, 5
    jne cont
    mov CS:time, 0
    
    cmp CS:counter, 23
    jne cont1
    mov CS:counter, 0
    cont1:
    
    mov CL, CS:counter
    cycle:
        new_line
    loop cycle
    
    print_mes "Hello, World "
    
    mov CL, CS:consol_height
    sub CL, CS:counter
    cycle1:
        new_line
    loop cycle1
    
    inc CS:counter
    
    cont:
    pop CX
    jmp dword ptr CS: [old_1Ch]
    counter DB 0
    time DB 0
    consol_height DB 24
new_1Ch endp
;===================================================================
old_1Ch DD ?
;===================================================================
BEGIN:
    get_vector 1Ch, old_1Ch
    set_vector 1Ch, new_1Ch

    mov CX, 0100h
    c:
        xor AX, AX
        go:
            inc AX
            cmp AX, 0
        jne go
    loop c
    recovery_vector 1Ch, old_1Ch

    print_mes 'END'
    mov AX, 4C00h
    int 21h

CODE_SEG ENDS
END START