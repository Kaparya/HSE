CODESG SEGMENT PARA 'CODE'
assume CS:CODESG, DS:CODESG
org	100h
start:
JMP	BEGIN
	CR	EQU	13
	LF	EQU	10
;=============================macro=================================
 print_letter	macro	letter
	push	AX
	push	DX
	mov	DL, letter
	mov	AH,	02
	int	21h
	pop		DX
	pop		AX
endm
;===================================================================
   print_mes	macro	message
   	local	msg, nxt
   	push	AX
   	push	DX
   	mov		DX,	offset msg
   	mov		AH,	09h
   	int	21h
   	pop		DX
    pop		AX
   	jmp nxt
msg	DB message,'$'
nxt:
 	endm
;===================================================================	
;===================================================================
Print_Word_dec	macro	src	, mes	;	выводит на экран источник src в десятичном виде
local	l1, l2, ex
;
push	AX
push	BX
push	CX
push	DX
;print_letter	CR
;print_letter	LF
print_mes	mes
	mov		AX,	src					;	Выводимое число в регисте EAX
	push		-1					;	Сохраним признак конца числа
	mov		cx,	10					;	Делим на 10
l1:	
	xor		dx,	dx					;	Очистим регистр dx 
	div		cx						;	Делим 
	push		DX						;	Сохраним цифру
	or 			AX,	AX				;	Остался 0? (это оптимальнее, чем  cmp	ax,	0 )
	jne		l1						;	нет -> продолжим
	mov		ah,	2h
l2:	
	pop		DX						;	Восстановим цифру
	cmp		dx,	-1					;	Дошли до конца -> выход {оптимальнее: or EDX,dx jl ex}
	je			ex
	add		dl,	'0'					;	Преобразуем число в цифру
	int		21h						;	Выведем цифру на экран
	jmp	l2							;	И продолжим
ex:	
pop		DX
pop		CX
pop		BX
pop		AX
;
endm
	BEGIN:

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
print_letter	CR
print_letter	LF
;
   mov CX,16
cycle:
    push    CX
    mov     CX,16
inside:
push	CX
call	rand8
pop	CX
loop	inside
 print_letter	CR
print_letter	LF
pop	CX
loop	cycle
;
int	20h
; rand8
; Возвращает случайное 8-битное число в AL.
; Переменная seed должна быть инициализирована заранее,
; например из области данных BIOS, как в примере для конгруэнтного генератора.
rand8	proc near
		mov	AX,		word ptr	seed
		mov	CX,		8	

newbit:	mov	BX,		AX
		and	BX,		002Dh
		xor	BH,	BL
		clc
		jpe	shift
		stc
shift:	rcr	AX,	1
	loop	newbit
	mov	word	ptr	seed,	AX
	mov	AH,	0
Print_Word_dec AX, ' '
	ret
rand8 endp
seed	dw 1
CODESG ends
  end start

  