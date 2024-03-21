code_seg segment
ASSUME CS:CODE_SEG,DS:code_seg,ES:code_seg
org 100h
start:
jmp begin

get_vector macro vector, old_vector
    push BX
    push ES
    push AX
    
    mov AX, 35&vector ; получить вектор прерывания
    int 21h
    mov word ptr old_vector, BX ; ES:BX - вектор
    mov word ptr old_vector+2, ES
    
    pop AX
    pop ES
    pop BX
endm

;===================================================================
set_vector macro vector, isr
    push DX
    push AX

    mov DX, offset isr ; получить смещение точки входа в новый обработчик на DX
    mov AX, 25&vector ; функция установки прерывания
    int 21h ; изменить вектор AL - номер прерыв. DS:DX - ук-ль программы обр. прер.
    
    pop AX
    pop DX
endm
;===================================================================
;===================================================================
;===================================================================
new_1Ch proc far
    push CX
    
    inc CS:time
    cmp CS:time, 18
    je cont1
    jmp cont

    print_message macro message
    local msg214, nxt
	push AX
	push DX
	
	mov DX, offset msg214
	mov AH, 09h
	int 21h
	
	pop DX
	pop AX
	
	
	jmp nxt
	msg214 DB message, '$'
	nxt:
    endm
    
    print_tetrada macro
    local print
	add AL, 030H
	cmp AL, 03AH
	jb print    
		       
	add AL, 07H  
	print:   
	int 10h
    endm
    
    print_byte macro
    local c1, break
	push DX
	push AX
	
	mov DL, AL
	xor AL, AL
	c1:
	    cmp DL, 0Ah
	    jb break
	    
	    add AL, 1
	    sub DL, 0Ah
	jmp c1
	break:
	                
	mov AH, 0Eh
	print_tetrada
	mov AL, DL
	print_tetrada
	
	pop AX
	pop DX
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
    
    cont1:
    mov CS:time, 0
    
    push AX
    push BX; сохранение используемых регистров в стеке
    push CX; сохранение используемых регистров в стеке
    push DX; сохранение используемых регистров в стеке
    push DS; сохранение используемых регистров в стеке

    push CS;настройка DS
    pop  DS;на наш сегмент, т.е DS=CS
;----------------------------------------------------------------------------
    
    mov AH, 2Ch
    int 21h
    
    mov curTime[0], CH
    mov curTime[1], CL
    mov curTime[2], DH
    
    mov AH, 03h
    mov BH, 0
    int 10h
    
    mov cursor[0], DH ;string
    mov cursor[1], DL ;column
    
    mov AH, 02h          ; Функция позиционирования
    mov BX, 0
    mov DH, 5   ; Строка
    mov DL, 60   ; Столбец
    int 10h

    mov AL, curTime[0]
    print_byte
    mov AH, 0Eh
    mov AL, ':'
    int 10h
    
    mov AL, curTime[1]
    print_byte
    mov AH, 0Eh
    mov AL, ':'
    int 10h
    
    mov AL, curTime[2]
    print_byte
    
    mov AH,02h          ; Функция позиционирования
    mov BX, 0
    mov DH, cursor[0]   ; Строка
    mov DL, cursor[1]   ; Столбец
    int 10h
    
    pop DS; восстановление регистров из стека в порядке LIFO
    pop DX
    pop CX
    pop BX
    pop AX
    
    cont:
    pop CX
    jmp dword ptr CS: [old_1Ch]
    
    cursor DB 2 dup(0)
    curTime DB 3 dup (0)
    time DB 0
new_1Ch endp
;===================================================================

;----------------------------------------------------------------------------
    int_2Fh_vector DD ?
    old_09h DD ?
    old_1Ch DD ?
    
    
    msg5 DB 'T', 'y', 'p', 'e', 13, 10, 0
    

    alarmHour DB 100 dup (0)
    alarmMins DB 100 dup (0)
    alarmCur DB 0
;----------------------------------------------------------------------------
    flag    DB 0

    page_num    DB 0
    SIZE1        DW 10
;============================================================================
new_09h proc far
;
    pushf
    push    AX
    in      AL,60h      ; Введем scan-code
    cmp     AL,58h      ; Это скен-код <F12>
    je      hotkey      ; Yes
    pop     AX          ; No. Восстановим AX
    popf
    jmp     dword ptr CS:[old_09h]  ; В системный обработчик без возврата
    hotkey:
	sti                 ; Не будем мешать таймеру
	in      AL,61h      ; Введем содержимое порта B
	or      AL,80h      ; Установим старший бит
	out     61h,AL      ; и вернем в порт B.
	and     AL,7Fh      ; Снова разрешим работу клавиатуры,
	out     61h,AL      ; сбросив старший бит порта B.
;
;-------------------- Вывод окна средствами BIOS ---------------------------
;
            push    BX  ; сохранение используемых регистров в стеке
            push    CX  ; сохранение используемых регистров в стеке
            push    DX  ; сохранение используемых регистров в стеке
	push  DS  ; сохранение используемых регистров в стеке
	;
	push  CS  ;  настройка DS
	pop    DS  ;        на наш сегмент, т.е DS=CS
      
	print_message 'Type the name of your alarm (HH:MM): '
	new_line
      
	pop    DS  ; восстановление регистров из стека в порядке LIFO
        pop     DX
        pop     CX
	pop     BX
;---------------------------------------------------------------------------
    cli
    mov     AL, 20h      ; Пошлем
    out     20h,AL       ; приказ EOI
; сброс для возможности прерываний с меньшим приоритетом
    pop     AX
  popf
    iret
;===========================================================================

;============================================================================
int_2Fh proc far
    cmp AH,0C7h     ; Наш номер?
    jne Pass_2Fh    ; Нет, на выход
    cmp AL,00h      ; Подфункция проверки на повторную установку?
    je inst         ; Программа уже установлена
    cmp AL,01h      ; Подфункция выгрузки?
    je unins        ; Да, на выгрузку
    jmp short Pass_2Fh ; Неизвестная подфункция - на выход
    inst:
        mov AL,0FFh ; Сообщим о невозможности повторной установки
        iret
    Pass_2Fh:
        jmp dword PTR CS:[int_2Fh_vector]
    ;
; -------------- Проверка - возможна ли выгрузка программы из памяти ? ------
    unins:
        push BX
        push CX
        push DX
        push ES
        ;
        mov CX,CS ; Пригодится для сравнения, т.к. с CS сравнивать нельзя
        mov AX,3509h ; Проверить вектор 09h
        int 21h ; Функция 35h в AL - номер прерывания. Возврат-вектор в ES:BX
        ;
        mov DX,ES
        cmp CX,DX
        jne Not_remove
        ;
        cmp BX, offset CS:new_09h
        jne Not_remove
        ;
        mov AX,352Fh ; Проверить вектор 2Fh
        int 21h ; Функция 35h в AL - номер прерывания. Возврат-вектор в ES:BX
        ;
        mov DX,ES
        cmp CX,DX
        jne Not_remove
        ;
        cmp BX, offset CS:int_2Fh
        jne Not_remove
; ---------------------- Выгрузка программы из памяти ---------------------
;
        push DS
    
	jmp contin2
	recovery_vector macro vector, old_vector
	    push DS
	    push DX
	    push AX
	    
	    lds DX, CS:old_vector
	    mov AX, 25&vector ; Заполнение вектора старым содержимым
	    int 21h ; DS:DX - указатель программы обработки пр
	    
	    pop AX
	    pop DX
	    pop DS
	endm
	contin2:
    
	recovery_vector 1Ch, CS:old_1Ch
        lds DX, CS:old_09h  ; Эта команда эквивалентна следующим двум
                            ; mov DX, word ptr old_09h
                            ; mov DS, word ptr old_09h+2
        mov AX,2509h ; Заполнение вектора старым содержимым
        int 21h

        lds DX, CS:int_2Fh_vector   ; Эта команда эквивалентна следующим двум
                                    ; mov DX, word ptr int_2Fh_vector
                                    ; mov DS, word ptr int_2Fh_vector+2
        mov AX,252Fh
        int 21h

        pop DS

        mov ES,CS:2Ch   ; ES -> окружение
        mov AH, 49h     ; Функция освобождения блока памяти
        int 21h

        mov AX, CS
        mov ES, AX  ; ES -> PSP выгрузим саму программу
        mov AH, 49h ; Функция освобождения блока памяти
        int 21h

        mov AL,0Fh  ; Признак успешной выгрузки
        jmp short pop_ret
    Not_remove:
        mov AL,0F0h ; Признак - выгружать нельзя
    pop_ret:
        pop ES
        pop DX
        pop CX
        pop BX
    ;
    iret
int_2Fh endp
;============================================================================
begin:
    
    mov CL,ES:80h   ; Длина хвоста в PSP
    cmp CL,0        ; Длина хвоста=0?
    je check_install; Да, программа запущена без параметров,
                    ; попробуем установить

    xor CH, CH          ; CX=CL= длина хвоста
    cld                 ; DF=0 - флаг направления вперед
    mov DI, 81h         ; ES:DI-> начало хвоста в PSP
    mov AL, ' '         ; Уберем пробелы из начала хвоста
    repe scasb          ; Сканируем хвост пока пробелы
                        ; AL - (ES:DI) -> флаги процессора
                        ; повторять пока элементы равны

    mov SI, offset key  ; DS:SI-> поле key
    dec DI      ; DI-> на первый символ после пробелов
    mov CX, 4   ; ожидаемая длина команды
    repe cmpsb  ; Сравниваем введенный хвост с ожидаемым
                ; (DS:DI)-(ES:DI) -> флаги процессора

    jne check_install; Неизвестная команда - попробуем установить
    inc flag_off
                        ; Проверим, не установлена ли уже эта программа
    check_install:
        mov AX,0C700h   ; AH=0C7h номер процесса C7h
                        ; AL=00h -дать статус установки процесса
        int 2Fh         ; мультиплексное прерывание
        cmp AL,0FFh
        je already_ins  ; возвращает AL=0FFh если установлена
;----------------------------------------------------------------------------
        cmp flag_off,1
        jne contewr
	jmp xm_stranno
;----------------------------------------------------------------------------
	contewr:
	
        mov AX,352Fh    ; получить вектор
        int 21h         ; прерывания 2Fh
        mov word ptr int_2Fh_vector,BX  ; ES:BX - вектор
        mov word ptr int_2Fh_vector+2,ES
        mov DX,offset int_2Fh   ; получить смещение точки входа в новый
                                ; обработчик на DX
        mov AX,252Fh            ; функция установки прерывания
                                ; изменить вектор 2Fh
        int 21h                 ; AL - номер прерыв. DS:DX - указатель программы обработки прер.
;============================================================================
        mov AX,3509h    ; получить вектор
        int 21h         ; прерывания 09h
        mov word ptr old_09h,BX     ; ES:BX - вектор
        mov word ptr old_09h+2,ES
        mov DX, offset new_09h      ; получить смещение точки входа в новый
                                    ; обработчик на DX
        mov AX,2509h    ; функция установки прерывания
                        ; изменить вектор 09h
        int 21h         ; AL - номер прерыв. DS:DX - указатель программы обработки прер.

        mov DX, offset msg1 ; Сообщение об установке
        call print
;----------------------------------------------------------------------------
	get_vector 1Ch, old_1Ch
	set_vector 1Ch, new_1Ch
	
        mov DX,offset begin ; оставить программу ...
        int 27h             ; ... резидентной и выйти
;============================================================================
already_ins:
    cmp flag_off, 1 ; Запрос на выгрузку установлен?
    je uninstall    ; Да, на выгрузку
    lea DX, msg     ; Вывод на экран сообщения: already installed!
    call print
    int 20h
; ------------------ Выгрузка -----------------------------------------------
uninstall:
    recovery_vector 1Ch, old_1Ch
    mov AX,0C701h   ; AH=0C7h номер процесса C7h, подфункция 01h-выгрузка
    int 2Fh         ; мультиплексное прерывание
    cmp AL,0F0h
    je not_sucsess
    cmp AL,0Fh
    jne not_sucsess
    mov DX,offset msg2  ; Сообщение о выгрузке
    call print
    
    int 20h
not_sucsess:
    mov DX,offset msg3  ; Сообщение, что выгрузка невозможна
    call print
    int 20h
xm_stranno:
    mov DX,offset msg4  ; Сообщение, программы нет, а пользователь
    call print          ; дает команду выгрузки
    int 20h
;----------------------------------------------------------------------------

    key DB '/off'
    flag_off DB 0
    msg  DB 'already '
    msg1 DB 'installed', 13, 10,'$'
    msg4 DB 'just '
    msg3 DB 'not '
    msg2 DB 'uninstalled', 13, 10,'$'
;============================================================================
PRINT PROC NEAR
    MOV AH,09H
    INT 21H
    RET
PRINT ENDP
;;============================================================================
code_seg ends
end start