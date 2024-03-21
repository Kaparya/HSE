code_seg segment
ASSUME CS:CODE_SEG,DS:code_seg,ES:code_seg
org 100h

start:
    jmp begin 
    
;=================================
Help DB 100, 0, 100 dup(0)
fileName DB 20 dup (0)
ourString DB 100 dup (0)
counter DB 8 dup (0)

prevBuffer DB 200 dup(0)
ourString_len DW 0

fl DB 0

handler DW ?
RealRead DW ?
buffer_len EQU 400 ;20480
Bufin DB buffer_len dup (?)

CR EQU 0Dh
LF EQU 0Ah
;=================================

counterAdd macro
local out_, next, next1, retur
    push DX
    push BX
    
    mov BX, 0
    
    next:
        cmp BX, 7
        je out_
        mov DL, CS:counter[BX]
        
        cmp DL, 0FFh
        jne out_
        
        inc BX
        
        jmp next
    
    out_:
        inc CS:counter[BX]
        
        next1:
            cmp BX, 0
            je retur
            
            dec BX
            
            xor DL, DL
            mov CS:counter[BX], DL
            
            jmp next1
            
    retur:
    
    pop BX
    pop DX
endm

print_tetrada proc
      add DL, 030H
      cmp DL, 03AH
      jb print     
                   
      add DL, 07H  
      print:       
      int 21H
      ret
endp

print_byte proc
      push DX            
      shr DL, 4          
      mov AH, 02H
      call print_tetrada
      pop DX             
      and DL,0Fh
      call print_tetrada
      ret
endp

printCounter macro
local next1, next2, retur, printFromCurrent
    push DX
    push BX
    
    mov BX, 7
    
    next2:
        mov DL, CS:counter[BX]
        
        cmp BX, 0
        je printFromCurrent
        cmp DL, 0
        je next1
    
        jmp printFromCurrent
    
        next1:
            
            dec BX
            jmp next2
        
    printFromCurrent:
        mov DL, CS:counter[BX]
        xor DH, DH
        
        push BX
        call print_byte
        pop BX
        
        cmp BX, 0
        je retur
        
        dec BX
            
        jmp printFromCurrent
        
    retur:

    pop BX
    pop DX
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

;=================================
;=================================
;=================================

begin:
    
    mov BL, ES:[80h]
    cmp BL, 0
    jne pspInput
    jmp simpleInput
    
;---------------------------------

pspInput:
    
    xor BH, BH
    mov BL, ES:[80h]
    mov byte ptr [BX + 81h], 0
    
    xor CH, CH
    mov CL, ES:[80h]
    cld
    mov DI, 81h
    mov AL, ' '
    repe scasb
    dec DI
    
    mov BX, 0 ;индекс по fileName
    
    c0:
        mov CL, ES:[DI]
        cmp CL, 0
        je break0
        
        cmp CL, ' '
        je pspSubstring
        
        mov fileName[BX], CL
        
        inc DI 
        inc BX
    jmp c0
    
    mov fileName[BX], 0
    
    pspSubstring:
        
        mov BX, 0h ;индекс по ourString
        
        c02: ;цикл, пропускающий пробелы
            mov CL, ES:[DI]
            cmp CL, ' '
            jne c01
            
            inc DI
        jmp c02
        
        c01:
            mov CL, ES:[DI]
            cmp CL, 0
            je break0
            cmp CL, ' '
            je break0
            
            mov ourString[BX], CL
            inc CS:ourString_len
            
            inc DI
            inc BX
        jmp c01        
        
    break0:
    mov CL, ourString[0]
    cmp CL, 0
    je cont1
    
    mov ourString[BX], '$'
    
    cont1:
    jmp openFile
    
;---------------------------------

simpleInput:
    
    print_message 'Type your file name: '
    mov AH, 0Ah
    mov DX, offset Help
    int 21h
    
    xor CH, CH
    mov CL, Help[1]
    mov BX, 2
    mov DX, 0 ;индекс по fileName (потом переписывается в BX)
    c1:
        mov AL, Help[BX]
        cmp AL, ' '
        je skip
        
        push BX
        mov BX, DX
        mov fileName[BX], AL
        pop BX
        inc DX
        
        skip:
        inc BX
    loop c1
    mov BX, DX
    mov fileName[BX], 0
    
;---------------------------------

openFile:
    
    mov AX, 3D02h ;открытие файла для чтения/записи
    mov DX, offset fileName
    int 21h
    
    jnc openOK
    
;---------------------------------

openError:

    new_line
    print_message 'Wrong input File Name!'
    new_line
    int 20h
    
;---------------------------------

openOK:

    mov handler, AX
    mov BL, ourString[0]
    cmp BL, 0              ; проверка, получили ли мы ourString до этого
    je inputSubstring      ; если нет, то просим от пользователя
    
    jmp cycle
    
;---------------------------------

inputSubstring:

    new_line
    print_message 'Type your substring (spaces are important):'
    new_line
    mov AH, 0Ah
    mov DX, offset Help
    int 21h
    
    xor CH, CH
    mov CL, Help[1]
    mov BX, 0
    c2:
        mov AL, Help[BX + 2]
        mov ourString[BX], AL
        inc CS:ourString_len
        inc BX
    loop c2
    mov ourString[BX], '$'
    
;---------------------------------

cycle:

    ReadFile handler, Bufin, buffer_len, RealRead
    
;---------------------------------

checkPrevBuffer:

    mov DL, prevBuffer[0]
    cmp DL, 0
    jne c60
    
    jmp countSubstrings
    
    c60:
    
    mov CX, 0 ;индекс по предыдущему буферу
    
    c6:
        push CX
        
        mov BX, 0 ;индекс по нашей строке
        
        c7:
            mov DX, CS:ourString_len
            dec DX
            cmp DX, CX ;проверка дошли ли мы до места начала нового буфера
            je c7precontinue
            
            push BX
            mov BX, CX
            mov DL, prevBuffer[BX]
            pop BX
            cmp DL, ourString[BX] ;символы не совпали, выходим из цикла
            jne c7break
            
            inc BX
            inc CX
            jmp c7
        
        c7precontinue:
            mov CX, 0 ;в CX индекс по новому буферу
            
        c7continue:
            mov DL, ourString[BX]
            cmp DL, '$' ;если дошли до конца нашей строки -> все
            je addOne1  ;предыдущие символы совпали
            
            mov DX, CS:ourString_len
            cmp DX, BX 
            je c7break
            
            cmp CX, CS:RealRead ;если буфер маленький -> строка не влезла
            je countSubstrings
            
            
            
            mov AX, BX
            mov DL, ourString[BX]
            
            mov BX, CX
            cmp DL, Bufin[BX]
            jne c7break ;если сиволы не совпали -> выход
            
            mov BX, AX
            
            inc BX
            inc CX
            jmp c7continue
            
        addOne1:
            counterAdd
        
        c7break:
        
        pop CX
        add CX, 2
        cmp CX, CS:ourString_len
        je countSubstrings
        dec CX
        
        jmp c6
        
;---------------------------------

countSubstrings:
    
    mov CX, 0 ; CX - индекс по буферу
    c3:
        push CX
        
        mov BX, 0 ; BX - индекс по ourString
        c4:
            cmp ourString[BX], '$' ; проверка, если это конец ourString
            je addOne              ; значит все предыдущие символы совпали
            cmp CX, RealRead
            je break1
            
            push BX
            mov BX, CX
            mov AL, Bufin[BX]
            pop BX
            
            cmp ourString[BX], AL
            jne break1
            
            inc BX
            inc CX
        jmp c4
        
        addOne:
            counterAdd
        
        break1:
    
        pop CX
        cmp CX, RealRead
        je c3break
        
        inc CX
    jmp c3
    
;---------------------------------
    
c3break:
    
    cmp RealRead, buffer_len
    jne printWord
    
    mov BX, buffer_len
    sub BX, CS:ourString_len
    inc BX
    
    mov CX, 0 ;индекс по prevBuffer
    
    c5: 
        cmp BX, buffer_len
        je c5break
        
        mov DL, byte ptr Bufin[BX]
        push BX
        mov BX, CX
        mov byte ptr prevBuffer[BX], DL
        pop BX
        
        inc BX
        inc CX
        
        jmp c5
    
    c5break:
    
    mov BX, CX
    mov prevBuffer[BX], '$'
    push CX
    
    ;new_line
    ;print_message "--"
    ;mov AH, 09h
    ;mov DX, offset prevBuffer
    ;int 21h
    ;print_message "--"
    ;mov DX, CS:ourString_len
    ;call print_byte
    
    pop CX
    mov BX, CX
    mov prevBuffer[BX], 0
    
    jmp cycle
    
;---------------------------------
printWord:
        
    new_line
    new_line
    print_message 'There are '
    printCounter
    print_message ' your substrings in file'
    new_line
    
mov AX, 4C00h
int 21h

;=================================
;=================================
;=================================

code_seg ends
end start