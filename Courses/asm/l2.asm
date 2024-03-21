CODE_SEG SEGMENT
ASSUME CS:CODE_SEG,DS:code_seg
ORG 100H 
START:
jmp begin

int_2Fh_vector DD ? 
old_09h DD ?
tail DW 0
flag        DB 9 dup(0)
;=============================================================================
;
;=============================================================================
new_09h proc far
;
    pushf
	push    AX
	
    in      AL,60h      ; Введем scan-code
	s1:
		cmp     AL,13h      ; Это скен-код  "R" ?
		jne s2
		cmp CS:flag[0],0
		jne s2 
		mov DL,0DAh        ; выводим символ уголка
		mov CS:flag[0],1
		jmp hotkey
	s2:
		cmp     AL,14h      ; Это скен-код "T" ?
		jne s3
		cmp CS:flag[1],0
		jne s3
		mov DL,0C4h          ; выводим символ "-"
		mov CS:flag[1],1
		jmp hotkey 
	
	s3:
		cmp     AL,15h      ; Это скен-код "Y" ?
		jne s4
		cmp CS:flag[2],0
		jne s4
		mov DL,0BFh         ; выводим символ уголка
		mov CS:flag[2],1
		jmp hotkey 
		 
	s4:
		cmp     AL,21h      ; Это скен-код "F" ?
		jne s5
		cmp CS:flag[3],0
		jne s5
		mov DL,0B3h        
		mov CS:flag[3],1
		jmp hotkey 
		 
	s5:
		cmp     AL,22h      ; Это скен-код "G" ?
		jne s6
		cmp CS:flag[4],0
		jne s6
		mov DL,0B0h
		mov CS:flag[4],1
		jmp hotkey 
		
		 
	s6:
		cmp     AL,23h      ; Это скен-код "H" ?
		jne s7
		cmp CS:flag[5],0
		jne s7
		mov DL,0B3h
		mov CS:flag[5],1
		jmp hotkey 
		
	s7:
		cmp     AL,2Fh      ; Это скен-код "V" ?
		jne s8
		cmp CS:flag[6],0
		jne s8
		mov DL,0C0h
		mov CS:flag[6],1
		jmp hotkey 
		
	s8:
		cmp     AL,30h      ; Это скен-код "B" ?
		jne s9
		cmp CS:flag[7],0
		jne s9
		mov DL,0C4h
		mov CS:flag[7],1
		jmp hotkey 
		
	s9:
		cmp     AL,31h      ; Это скен-код "N" ?
		jne next
		cmp CS:flag[8],0
		jne next
		mov DL,0D9h
		mov CS:flag[8],1
		jmp hotkey 
		
	next:	
		pop     AX          ; No. Восстановим AX
		popf
		jmp     dword ptr CS:[old_09h]  ; В системный обработчик без возврата
hotkey:
    sti                 ; Не будем мешать таймеру
    in      AL,61h      ; Введем содержимое порта B
    or      AL,80h      ; Установим старший бит
	
	mov AH,02h
	int 21h
    out     61h,AL      ; и вернем в порт B.
    and     AL,7Fh      ; Снова разрешим работу клавиатуры,
    out     61h,AL      ; сбросив старший бит порта B.
	jmp prodolzhenie
;

;---------------------------------------------------------------------------
prodolzhenie:
    cli
    mov     AL, 20h      ; Пошлем
    out     20h,AL       ; приказ EOI
;
    pop     AX
	popf
    iret
new_09h     endp
;=============================================================================
;
;=============================================================================
int_2Fh proc far
cmp AX,0C700h
jne pass_2Fh
mov AL,0FFh
iret
pass_2Fh:
jmp dword PTR CS:[int_2Fh_vector] 
int_2Fh endp
;=============================================================================
;
;=============================================================================
begin:
mov AX,0C700h ; AH=OC7h номер процесса C7h
; AL=OOh -дать статус установки процесса
int 2Fh ; мультиплексное прерывание 

cmp AL, 0
jz not_installed ; возвращает AL=0 если не установлена
lea DX,msg 
call print 
int 20h 
msg DB 'Already installed',13,10,'$'
not_installed:

mov AX,352Fh ; получить вектор
int 21h ; прерывания 2Fh 
mov word ptr int_2Fh_vector,BX ; ES:BX - вектор
mov word ptr int_2Fh_vector+2,ES 
mov DX,offset int_2Fh ; получить смещение точки входа в новый 
; обработчик на DX
mov AX,252Fh ; функция установки прерывания 
; изменить вектор 2Fh
int 21h ; AL - номер прерыв. DS:DX - указатель программы обработки прер.
mov AX,3509h ; получить вектор
int 21h ; прерывания 09h
mov word ptr old_09h,BX ; ES:BX - вектор
mov word ptr old_09h+2,ES ;
mov DX,offset new_09h ; получить смещение точки входа в новый
; обработчик на DX
mov AX,2509h ; функция установки прерывания 
; изменить вектор 09h
int 21h ; AL - номер прерыв. DS:DX - указатель программы обработки прер.
;------------------------------------------------------------------------------
mov DX,offset begin ; оставить программу ...
int 27h ; ... резидентной и выйти
;=============================================================================
;
;=============================================================================
PRINT PROC NEAR
MOV AH,09H
INT 21H
RET 
PRINT ENDP
;=============================================================================
CODE_SEG ENDS 
END START