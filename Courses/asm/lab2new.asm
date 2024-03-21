;============================= УПРАВЛЕНИЕ =================================
; F9 - список запущенных TSR программ
;
; F10 - удаление вектора (ручное)
; Если в уже запущенной TSR программе несколько векторов заменяются, то 
; при удалении всего 
; одного из них - она просто перестанет работать, при этом занимая другие
; вектора, поэтому при выборе вектора, удаляются все вектора из той же 
; TSR программы
; 
; F12 - вывод перехваченных векторов (в скобках порядковый 
; номер соответствующей программы)
;==========================================================================
;
; Каждый вектор может заменятся всего один раз, нет возможности заменять
; заменный вектор (тогда перестает работать предыдущая программа). 
;
;======================== ДЛЯ ЧЕГО ЗАМЕНЕНЫ ВЕКТОРА =======================
; 09h - проверка нажатия клавиш
;
; 21h - установка / удаление вектора
;       если 4Bh, то запоминаем имя программы в буффер progName
;
; 27h - программа осталась резидентной, поэтому можно записывать ее имя в 
;       список запущенных программ progs (массив, где с каждого кратного 20
;	индекса начало нового имени программы)
;
; 2Fh - проверка установлена ли уже программа
;==========================================================================

code_seg segment
ASSUME CS:CODE_SEG,DS:code_seg,ES:code_seg
org 100h
start:
jmp begin

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
    
print_tetradaWind macro		; вывод тетрады через 10h
local print
    add AL, 030H
    cmp AL, 03AH
    jb print    
		       
    add AL, 07H  
    print:   
    int 10h
endm
    
print_byteWind macro		; вывод байта через 10h
local c1, break
    push DX
    push AX
	   
    mov AH, 0Eh
    push AX
    
    shr AL, 4      
    print_tetradaWind
    
    pop AX
    and AL, 0Fh
    print_tetradaWind
	
    pop AX
    pop DX
endm
    
print_byteDecWind macro		; вывод байта в десятичном виде через 10h
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
    print_tetradaWind
    mov AL, DL
    print_tetradaWind
	
    pop AX
    pop DX
endm
    
new_lineWind macro		; новая строка через 10h (на столбце для 
    push DX			; красного прямоугольника)
    push BX
    push AX
	
    mov AH, 03h
    mov BH, 0
    int 10h
	    
    mov AH,02h
    mov BX, 0
    inc DH
    mov DL, 62
    int 10h
	    
    pop AX
    pop BX
    pop DX
endm

new_lineWind1 macro		; новая строка через 10h (на столбце для  
    push DX			; зеленого прямоугольника)
    push BX
    push AX
	
    mov AH, 03h
    mov BH, 0
    int 10h
	    
    mov AH,02h
    mov BX, 0
    inc DH
    mov DL, 37
    int 10h
	    
    pop AX
    pop BX
    pop DX
endm
;==========================================================================
;============================== ПЕРЕМЕННЫЕ ================================
;==========================================================================
    old_2Fh DD ?
    old_09h DD ?
    old_1Ch DD ?
    old_21h DD ?
    old_27h DD ?
;--------------------------------------------------------------------------
    flag    DB 0

    page_num    DB 0
    cursor DB 2 dup (0)
    cursor1 DB 2 dup (0)
    SIZE1        DW 10
    
    newVectors DB 21h, 99 dup (0)
    newVectorsSize DW 1
    newVectorsOld DD 100 dup (0)
    newVectorsProg DB 100 dup (0)
    
    progs DB 2000 dup(0)
    progsSize DW 0
    progName DB 'lab2new.COM', 9 dup (0)
;==========================================================================
new_09h proc far

    pushf
    push    AX
    in      AL,60h		; Введем scan-code
    cmp     AL,58h		; Это <F12> => вывод списка векторов
    je      hotkey
    cmp     AL,44h		; Это <F10> => предложение убрать вектор
    jne     skip1
    jmp     hotkey1
    
    skip1:
    cmp AL, 43h			; Это <F9>  => вывод списка команд
    jne skip3
    jmp hotkey2
    
    skip3:
	pop     AX
	popf
	jmp     dword ptr CS:[old_09h]
;--------------------------------------------------------------------------
    hotkey:
	sti			; Не будем мешать таймеру
	in      AL,61h		; Введем содержимое порта B
	or      AL,80h		; Установим старший бит
	out     61h,AL		; и вернем в порт B.
	and     AL,7Fh		; Снова разрешим работу клавиатуры,
	out     61h,AL		; сбросив старший бит порта B.

;-------------------- Вывод списка замененных векторов --------------------

	push BX
	push CX
	push DX
	push DS
	
	push CS			; настройка DS
	pop  DS			; на наш сегмент, т.е DS=CS
      
      
	mov AH, 03h
	mov BH, 0
	int 10h
	
	mov cursor[0], DH ;string
	mov cursor[1], DL ;column
	
	mov AX, 0600h		; выведем окно (красное)
	mov BH, 40h
	mov CH, 4
	mov CL, 58
	mov DH, 20
	mov DL, 78
	int 10h
	
	mov AH, 02h		; Функция позиционирования
	mov BX, 0
	mov DH, 5  ; Строка
	mov DL, 60 ; Столбец
	int 10h
	  
	mov AX, newVectorsSize
	print_byteDecWind
	print_message ' vectors changed'
	new_lineWind
      
	xor BX, BX
	c1:
	    cmp BX, newVectorsSize
	    jne skip5
	    jmp break_
	    
	    skip5:
	    
	    mov AL, BL		; вывод порядкового номера
	    inc AL
	    print_byteDecWind
	    
	    print_message '. '  ; вывод номера вектора
	    mov AL, newVectors[BX]
	    print_byteWind
	    
	    print_message ' ('
	    inc CS:newVectorsProg[BX]
	    mov AL, CS:newVectorsProg[BX] ; вывод номера программы
	    dec CS:newVectorsProg[BX]     ; соответсвующей вектору
	    print_byteDecWind
	    print_message ')'
	    
	    new_lineWind
	    
	    inc BL
	jmp c1
	
	break_:
	
	mov AH,02h		; Функция позиционирования
				; (возврат курсора)
	mov BX, 0
	mov DH, cursor[0] ; Строка
	mov DL, cursor[1] ; Столбец
	int 10h
      
	pop     DS
        pop     DX
        pop     CX
	pop     BX
	
	jmp out_
;--------------------------------------------------------------------------
    hotkey1:
    
	sti			; Не будем мешать таймеру
	in      AL,61h		; Введем содержимое порта B
	or      AL,80h		; Установим старший бит
	out     61h,AL		; и вернем в порт B.
	and     AL,7Fh		; Снова разрешим работу клавиатуры,
	out     61h,AL		; сбросив старший бит порта B.
	
	cli
	mov     AL, 20h		; Пошлем
	out     20h,AL		; приказ EOI
    ; сброс для возможности прерываний с меньшим приоритетом

;------------ Вывод предложения заменить вектор на начальный --------------
	
	push AX
	push SI
	push BX
	push CX
	push DX
	push DS
	
	push CS
	pop  DS
	
	print_message 'Do you want to uninstall any vectors? (y/n) '
	mov AH, 01h
	int 21h
	
	cmp AL, 'y'		; если пользователь согласен - продолжаем
	je cont1
	jmp theEnd
	
	buffer DB 3, 3 dup(0)
	
	cont1:
	
	cmp CS:newVectorsSize, 4
	je cont2
	jmp skip2
	cont2:
	new_line                ; если остались только вектора программы 
	new_line		; удалить их не можем
	print_message '*WARNING* You cant remove your programs vectors'
	new_line
	print_message '*WARNING* Use key /off to uninstall this vectors'
	new_line
	jmp theEnd
	skip2:
	
	new_line
	new_line
	print_message '*WARNING* All vectors connected with this program will be removed'
	new_line
	new_line
	print_message 'Type the DEC number of the vector you want to remove: '
	
	mov DX, offset buffer
	mov AH, 0Ah
	int 21h
	new_line
	
	xor BH, BH
	mov BL, 2
	add CS:buffer[1], 2     ; перевод строки в число, если возможно
	xor AX, AX
	mov CL, 0Ah
	c2:
	
	    cmp BL, CS:buffer[1]
	    jae break_1
	    
	    cmp CS:buffer[BX], 30h
	    jb wrongSymb
	    
	    cmp CS:buffer[BX], 39h
	    ja wrongSymb
	    
	    mul CL
	    add AL, CS:buffer[BX]
	    sub AL, 30h
	    
	    inc BL
	jmp c2
;==========================================================================
        wrongSymb:
	
	    print_message 'WRONG NUMBER!'
	    jmp theEnd
	    
;==========================================================================
	break_1:

	cmp AX, CS:newVectorsSize
	ja wrongSymb
	cmp AX, 0
	je wrongSymb
	cmp AX, 5
	jae skip4
				; мы не можем удалить вектора нашей
				; программы
	    print_message 'You cant remove your programmes vectors!'
	    new_line
	    print_message 'Use key /off to uninstall it'
	    jmp theEnd
	    
	skip4:
	
	mov SI, AX
	dec SI
	
	mov CL, CS:newVectorsProg[SI]
	
	c12:			; проход назад (поиск первого вектора 
				; из той же программы)
	
	    cmp SI, 0
	    je break_c12
	    
	    cmp CS:newVectorsProg[SI], CL
	    jne break_c12
	    
	    dec SI
	
	jmp c12
	
	break_c12:
	
	cmp CS:newVectorsProg[SI], CL
	je skip11		; если ушли дальше чем нужно, то + 1
	inc SI
	skip11:
	
	mov CS:flag, 0		; флаг, для удаления программы один раз
	push DS
	mov DI, CX
	
	c13:			; удаление всех векторов из той же
				; программы
	
	    push DI
	    push CX
	    cmp SI, CS:newVectorsSize
	    je break_c13
	    mov CX, DI
	    cmp CS:newVectorsProg[SI], 0
	    je doIt
	    cmp CS:flag, 0	; первый раз точно нужный вектор
	    je doIt		; потом - они просто меньше чем номер
				; нашей программы 
				
				; в силу отсортированности векторов 
				; по номерам программ
	    cmp CS:newVectorsProg[SI], CL
	    jae break_c13
	    
	    doIt:
	    pop CX
	    pop DI
	    
	    
	    push CX
	    push SI
	    push DI
	    lds DX, CS:newVectorsOld[SI]
	    mov AH,25h		; Заполнение вектора старым содержимым
	    mov AL, CS:newVectors[SI]
	    int 21h
	    pop DI
	    pop SI
	    pop CX
	    
	    inc CX
	
	jmp c13
	break_c13:
	pop CX
	pop DI
	
	mov CS:flag, 0
	
	pop DS
	
	new_line
	print_message 'Vector(-s) removed successfully'
	new_line
      
	theEnd:
	new_line
	pop DS
        pop DX
        pop CX
	pop BX
	pop SI
	pop AX
	
	pop AX
	popf
	iret
;--------------------------------------------------------------------------
    
    hotkey2:
    
	sti			; Не будем мешать таймеру
	in      AL,61h		; Введем содержимое порта B
	or      AL,80h		; Установим старший бит
	out     61h,AL		; и вернем в порт B.
	and     AL,7Fh		; Снова разрешим работу клавиатуры,
	out     61h,AL		; сбросив старший бит порта B.
	
	cli
	mov     AL, 20h		; Пошлем
	out     20h,AL		; приказ EOI
    ; сброс для возможности прерываний с меньшим приоритетом

;-------------------- Вывод списка запушенных программ --------------------
	
	push AX
	push SI
	push BX
	push CX
	push DX
	push DS
	
	push CS			; настройка DS
	pop  DS			; на наш сегмент, т.е DS=CS
	
	mov AH, 03h		; запомним где был курсор + сохраним
	mov BH, 0
	int 10h
	
	mov cursor1[0], DH ;string
	mov cursor1[1], DL ;column
	
	mov AX, 0600h		; выведем окно (зеленое)
	mov BH, 20h
	mov CH, 4
	mov CL, 33
	mov DH, 20
	mov DL, 57
	int 10h
	
	mov AH, 02h		; Функция позиционирования
	mov BX, 0
	mov DH, 5  ; Строка
	mov DL, 35 ; Столбец
	int 10h
	  
	mov AX, progsSize
	print_byteDecWind
	print_message ' programs run:'
	new_lineWind1
      
	xor BX, BX
	c6:
	    cmp BX, progsSize
	    jne skip10
	    jmp break_c6
	    
	    skip10:
	    
	    mov AL, BL
	    inc AL
	    print_byteDecWind   ; вывод порядкового номера
	    
	    print_message '. '
	    
	    push SI
	    mov AX, BX
	    push BX
	    mov BX, 14h
	    mul BX
	    mov SI, AX
	    pop BX
	    
	    mov AH, 0Eh
	    c7:
		mov AL, progs[SI]
		cmp AL, 0
		je break_c7
		
		int 10h		; вывод имени программы
		
		inc SI
	    
	    jmp c7
	    
	    break_c7:
	    
	    pop SI
	    
	    new_lineWind1
	    
	    inc BL
	jmp c6
	
	break_c6:
	
	mov AH,02h		; Функция позиционирования (вернуть курсор)
	mov BX, 0
	mov DH, cursor1[0] ; Строка
	mov DL, cursor1[1] ; Столбец
	int 10h    
    
	pop DS
        pop DX
        pop CX
	pop BX
	pop SI
	pop AX
	
	pop AX
	popf
	iret
;--------------------------------------------------------------------------
    out_:
    cli
    mov AL, 20h			; Пошлем
    out 20h, AL			; приказ EOI
; сброс для возможности прерываний с меньшим приоритетом
    pop     AX
    popf
    iret
;==========================================================================
;==========================================================================
new_2Fh proc far
    mov CS:flag, 0
    cmp AH, 0C7h		; Наш номер?
    jne Pass_2Fh		; Нет, на выход
    cmp AL, 00h			; Подфункция проверки на 
				; повторную установку?
				
    je inst			; Программа уже установлена
    cmp AL, 01h			; Подфункция выгрузки?
    je unins			; Да, на выгрузку
    jmp short Pass_2Fh		; Неизвестная подфункция - на выход
    inst:
        mov AL, 0FFh		; Сообщим о невозможности повторной установки
        iret
    Pass_2Fh:
        jmp dword PTR CS:[old_2Fh]
    
;---------- Проверка - возможна ли выгрузка программы из памяти ? ---------
    
    unins:
        push BX
        push CX
        push DX
        push ES
        
        mov CX,CS
        mov AX,3509h
        int 21h
        
        mov DX,ES
        cmp CX,DX
        jne Not_remove
        
        cmp BX, offset CS:new_09h
        jne Not_remove
        
        mov AX,352Fh		; Проверить вектор 2Fh
        int 21h			; Функция 35h в AL - номер прерывания.
				; Возврат-вектор в ES:BX
    
        mov DX,ES
        cmp CX,DX
        jne Not_remove
        ;
        cmp BX, offset CS:new_2Fh
        jne Not_remove
;----------------------- Выгрузка программы из памяти ---------------------

        push DS

;--------------------------------------------------------------------------
	lds DX, CS:old_21h
        mov AX,2521h		; Заполнение вектора старым содержимым
        int 21h
;--------------------------------------------------------------------------
	lds DX, CS:old_27h
        mov AX,2527h		; Заполнение вектора старым содержимым
        int 21h
;--------------------------------------------------------------------------
        lds DX, CS:old_09h
        mov AX,2509h		; Заполнение вектора старым содержимым
        int 21h
;--------------------------------------------------------------------------
        lds DX, CS:old_2Fh
        mov AX,252Fh		; Заполнение вектора старым содержимым
        int 21h
;--------------------------------------------------------------------------
        pop DS

        mov ES,CS:2Ch		; ES -> окружение
        mov AH, 49h		; Функция освобождения блока памяти
        int 21h

        mov AX, CS
        mov ES, AX		; ES -> PSP выгрузим саму программу
        mov AH, 49h		; Функция освобождения блока памяти
        int 21h

        mov AL,0Fh		; Признак успешной выгрузки
        jmp short pop_ret
    Not_remove:
        mov AL,0F0h		; Признак - выгружать нельзя
    pop_ret:
        pop ES
        pop DX
        pop CX
        pop BX
    
    iret
new_2Fh endp
;==========================================================================
new_27h proc far 
    
    push AX
    push DX			; нужен для запоминания имени программы
    push SI			; в массив (запомниается только 
    push DI			; если осталась резидентной)
    
    mov SI, 0
    
    mov AX, CS:progsSize
    mov DX, 14h
    mul DX
    
    mov DI, AX
    c9:
	mov AL, CS:progName[SI]
	mov CS:progs[DI], AL
	cmp AL, 0
	je break_c9
	
	inc SI
	inc DI
	
    jmp c9
    
    break_c9:
    
    inc CS:progsSize
    mov CS:flag, 0
    
    pop DI
    pop SI
    pop DX
    pop AX
    
    jmp dword ptr CS:[old_27h]

endp
;==========================================================================
new_21h proc far

    cmp AH, 25h
    je installVector
    cmp AH, 4Bh
    jne skip6
    jmp progLaunch
    skip6:
	jmp     dword ptr CS:[old_21h]
;--------------------------------------------------------------------------

    installVector:
	
	push AX
	push BX
	push DX
	push DS
	push ES
	push SI
	
	xor SI, SI
	
	c4:			; ищем, изменен ли этот вектор уже
	    cmp SI, CS:newVectorsSize
	    jb skip8
	    jmp vectorInstall
	    skip8:
	    
	    cmp AL, CS:newVectors[SI]
	    je vectorRemove
	
	    inc SI
	jmp c4
	
	vectorRemove:		; если да - то удаляем
	
	xor AH, AH
	mov AL, CS:newVectorsProg[SI]
	mov DI, AX
	
	cmp CS:flag, 0		; если флаг = 0, то удаляем 
				; программу из списка
	jne break_c11
	dec CS:progsSize
	
	inc CS:flag
	mov DX, 14h
	mul DX
	mov BX, AX
	
	mov AX, CS:progsSize
	mov DX, 14h
	mul DX
	mov CX, AX
	
	c10:			; сдвигаем имена программ
	    cmp BX, CX
	    jae break_c10
	    
	    mov DL, CS:progs[BX + 14h]
	    mov CS:progs[BX], DL
	    
	    inc BX
	
	jmp c10
	
	break_c10:
	
	mov BX, 0
	mov CX, CS:newVectorsSize
	
	c11:			; уменьшаем все номера программ
				; (сдвиг на единичку)
	    cmp BX, CX
	    jae break_c11
	    
	    mov AX, DI
	    cmp CS:newVectorsProg[BX], AL
	    jb skip7
	    cmp CS:newVectorsProg[BX], 0
	    je skip7
	    dec CS:newVectorsProg[BX]
	    skip7:
	    
	    inc BX
	
	jmp c11
	break_c11:
	
	dec CS:newVectorsSize
	
	c5:			; сдвигаем все значения в массивах
	    cmp SI, CS:newVectorsSize
	    jae break_c5
	    
	    mov DL, CS:newVectors[SI + 1]
	    mov CS:newVectors[SI], DL
	    
	    mov DL, CS:newVectorsProg[SI + 1]
	    mov CS:newVectorsProg[SI], DL
	    
	    push ES
	    mov BX, word ptr CS:newVectorsOld[SI + 1]
	    mov ES, word ptr CS:newVectorsOld[SI + 1] + 2
	    mov word ptr CS:newVectorsOld[SI], BX     ; ES:BX - вектор
	    mov word ptr CS:newVectorsOld[SI] + 2, ES
	    pop ES
	
	    inc SI
	jmp c5
	
	break_c5:
	
	pop SI
	pop ES
	pop DS
	pop DX
	pop BX
	pop AX
	
	jmp     dword ptr CS:[old_21h]
;--------------------------------------------------------------------------
	vectorInstall:		; если еще не установлен вектор - ставим
	
	xor SI, SI
	add SI, CS:newVectorsSize
	
	mov AH,35h		; получить вектор
        int 21h			; прерывания 09h
        mov word ptr CS:newVectorsOld[SI],BX
        mov word ptr CS:newVectorsOld[SI]+2,ES
	
	mov DX, CS:progsSize
	mov CS:newVectorsProg[SI], DL
	
	mov CS:newVectors[SI], AL
	inc CS:newVectorsSize
	
	pop SI
	pop ES
	pop DS
	pop DX
	pop BX
	pop AX
	
	jmp     dword ptr CS:[old_21h]
;==========================================================================
    
    progLaunch:			; когда программа запускается 
				; запоминаем имя
	
	push AX
	push BX
	push DX
	push DI
	push DS
	push ES
	push SI
	
	
	mov SI,DX
	
	mov DI, offset CS:progName
	
	c8:  
	    mov	AL, [SI]
	    mov	CS:[DI], AL
	    
	    cmp	AL,0
	    je	break_c8
	    
	    inc SI
	    inc	DI
;
	jmp c8
	
	break_c8:
	mov CS:flag, 0
	
	pop SI
	pop ES
	pop DS
	pop DI
	pop DX
	pop BX
	pop AX
    
    jmp     dword ptr CS:[old_21h]  ; В системный обработчик без возврата

endp
;==========================================================================
begin:
    
    mov CL,ES:80h		; Длина хвоста в PSP
    cmp CL,0			; Длина хвоста=0?
    je check_install		; Да, программа запущена без параметров,
				; попробуем установить
    
    xor CH, CH
    cld				; DF=0 - флаг направления вперед
    mov DI, 81h
    mov AL, ' '			; Уберем пробелы из начала хвоста
    repe scasb			; Сканируем хвост пока пробелы
				; AL - (ES:DI) -> флаги процессора
				; повторять пока элементы равны

    mov SI, offset key		; DS:SI-> поле key
    dec DI			; DI-> на первый символ после пробелов
    mov CX, 4			; ожидаемая длина команды
    repe cmpsb			; Сравниваем введенный хвост с ожидаемым
				; (DS:DI)-(ES:DI) -> флаги процессора

    jne check_install		; Неизвестная команда - попробуем установ
    inc flag_off
				; Проверим, не установлена ли уже эта программа
    check_install:
        mov AX,0C700h		; AH=0C7h номер процесса C7h
				; AL=00h -дать статус установки процесса
        int 2Fh			; мультиплексное прерывание
        cmp AL,0FFh
        je already_ins		; возвращает AL=0FFh если установлена
;--------------------------------------------------------------------------
        cmp flag_off,1
        jne contewr
	jmp xm_stranno
;--------------------------------------------------------------------------
	contewr:
;==========================================================================
	mov AX, 3521h		; устанавливаем вектора
	int 21h
        mov word ptr old_21h,BX  ; ES:BX - вектор
        mov word ptr old_21h+2,ES
	
	mov AX, 2521h
	mov DX, offset new_21h
	int 21h
;==========================================================================
	mov AX, 3527h		; устанавливаем вектора
	int 21h
        mov word ptr old_27h,BX
        mov word ptr old_27h+2,ES
	
	mov AX, 2527h
	mov DX, offset new_27h
	int 21h
;==========================================================================
        mov AX,352Fh		; устанавливаем вектора
        int 21h
        mov word ptr old_2Fh,BX
        mov word ptr old_2Fh+2,ES
	
	
        mov DX,offset new_2Fh
        mov AX,252Fh
        int 21h
;==========================================================================
        mov AX,3509h		; устанавливаем вектора
        int 21h
        mov word ptr old_09h,BX
        mov word ptr old_09h+2,ES
        mov DX, offset new_09h
        mov AX,2509h
        int 21h

        mov DX, offset msg1
        call print
;--------------------------------------------------------------------------
        mov DX,offset begin     ; оставить программу ...
        int 27h			; ... резидентной и выйти
;==========================================================================
already_ins:
    cmp flag_off, 1		; Запрос на выгрузку установлен?
    je uninstall		; Да, на выгрузку
    lea DX, msg			; Вывод на экран сообщения: 
				; already installed!
    call print
    int 20h
;------------------------------ Выгрузка ----------------------------------
uninstall:
    mov AX,0C701h		; AH=0C7h номер процесса C7h, 
				; подфункция 01h-выгрузка
    int 2Fh			; мультиплексное прерывание
    cmp AL,0F0h
    je not_sucsess
    cmp AL,0Fh
    jne not_sucsess
    mov DX,offset msg2		; Сообщение о выгрузке
    call print
    
    int 20h
not_sucsess:
    mov DX,offset msg3		; Сообщение, что выгрузка невозможна
    call print
    int 20h
xm_stranno:
    mov DX,offset msg4		; Сообщение, программы нет, а пользователь
    call print			; дает команду выгрузки
    int 20h
;--------------------------------------------------------------------------
    key DB '/off'
    flag_off DB 0
    msg  DB 'already '
    msg1 DB 'installed', 13, 10,'$'
    msg4 DB 'just '
    msg3 DB 'not '
    msg2 DB 'uninstalled', 13, 10,'$'
;==========================================================================
PRINT PROC NEAR
    MOV AH,09H
    INT 21H
    RET
PRINT ENDP
;==========================================================================
code_seg ends
end start