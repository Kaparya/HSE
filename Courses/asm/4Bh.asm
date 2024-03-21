;*******************************************************************************************************
;
;	This TSR programm types name of LOAD and EXEC programm
;
;*******************************************************************************************************
code_seg segment
	assume cs:code_seg,ds:code_seg,ss:code_seg
	org 100h
.286
;
start:
	jmp begin
	int_2Fh_vector  DD  ?
	old_21h         DD  ?
;
	prog_name		DB	80 dup (' ')
;
;*******************************************************************************************************
new_21h proc	far
	pushf
	pusha
	push 	ES
	push	DS
;---------------------
	push	CS
	pop		ES			;	ES=CS
;---------------------
	cmp AH, 4Bh
	je		ExecFunction
	jmp		OtherFunc
;
ExecFunction:
;
	xor CX,CX
    mov SI,DX					;	in DS:DX ASCIIZ str wiht prog name
	mov	DI,	offset	CS:prog_name
;------------------------------------------ cycle -----------------------------------
next:  
	mov	AL,			[SI]			; �������� � AL ��������� ������
;      		
	mov	CS:[DI],	AL				; �������� ������ �� AL � prog_name
;
	cmp	AL,0
	je	cycle_br
	;
	inc 	SI
	inc		DI
	inc CX
;
	jmp short next	
;
;	
;------------------------------------------ type prog name -----------------------------------
cycle_br:
;	
	mov	DH,10								; dh = y coord
	mov	DL,28								; dl = x coord
	mov AH, 13h								; Function Printing Message
	mov AL, 0h								; regime recording
	mov BH, 0h								; type page 
	mov BL, 04h								; description color background and font  (red letter on black)
mov	BP,	offset	CS:prog_name				; address of message in ES:BP
	int 10h									; draw string at dh:dl
;
OtherFunc:
	pop		DS
	pop		ES
	popa
	popf
	jmp 	dword PTR CS:[old_21h]
	;
new_21h endp
;**************************************************************************
;==========================================================================
int_2Fh proc far
    cmp     AH,0C8h         ; ��� �����?
    jne     Pass_2Fh        ; ���, �� �����
    cmp     AL,00h          ; ���������� �������� �� ��������� ���������?
    je      inst            ; ��������� ��� �����������
    cmp     AL,01h          ; ���������� ��������?
    je      unins           ; ��, �� ��������
    jmp     short Pass_2Fh  ; ����������� ���������� - �� �����
inst:
    mov     AL,0FFh         ; ������� � ������������� ��������� ���������
    iret
Pass_2Fh:
    jmp dword PTR CS:[int_2Fh_vector]
;
; -------------- �������� - �������� �� �������� ��������� �� ������ ? ------
unins:
    push    BX
    push    CX
    push    DX
    push    ES
;
    mov     CX,CS   ; ���������� ��� �����., �.�. � CS ���������� ������
;----------------------------------------------------------------------------
    mov     AX,352Fh    ; ��������� ������ 2Fh
    int     21h ; �-� 35h � AL - ����� ����������. �������-������ � ES:BX
;
    mov     DX,ES
    cmp     CX,DX
    jne     Not_remove
;
    cmp     BX, offset CS:int_2Fh
    jne     Not_remove
;----------------------------------------------------------------------------
;----------------------------------------------------------------------------
    mov     AX,3521h    ; ��������� ������ 21h
    int     21h ; �-� 35h � AL - ����� ����������. �������-������ � ES:BX
;
    mov     DX,ES
    cmp     CX,DX
    jne     Not_remove
;
    cmp     BX, offset CS:new_21h
    jne     Not_remove
;----------------------------------------------------------------------------
;----------------------------------------------------------------------------
; ---------------------- �������� ��������� �� ������ ---------------------
;
    push    DS
;
;
    lds     DX, 	CS:int_2Fh_vector   ; ��� ������� ���-�� ��������� ����
;    mov     DX, word ptr int_2Fh_vector
;    mov     DS, word ptr int_2Fh_vector+2
    mov     AX,	252Fh
    int     21h
;
;
   lds     DX, 	CS:old_21h   
    mov     AX,	2521h
    int     21h
;
;
    pop     DS
;
    mov     ES,	CS:2Ch       ; ES -> ���������
    mov     AH, 	49h         ; ������� ������������ ����� ������
    int     21h
;
    mov     AX, 	CS
    mov     ES, 	AX          ; ES -> PSP �������� ���� ���������
    mov     AH, 	49h         ; ������� ������������ ����� ������
    int     21h
;
    mov     AL,0Fh          ; ������� �������� ��������
    jmp     short pop_ret
Not_remove:
    mov     AL,0F0h          ; ������� - ��������� ������
pop_ret:
    pop     ES
    pop     DX
    pop     CX
    pop     BX
;
    iret
int_2Fh endp
;============================================================================
;============================================================================
begin:
	mov 	CL,	ES:80h       	; ����� ������ � PSP
	cmp 	CL,	0            ; ����� ������=0?
	je  	check_install   	; ��, ��������� �������� ��� ����������,
								; ��������� ����������
   xor 	CH,	CH       		; CX=CL= ����� ������
   cld             			; DF=0 - ���� ����������� ������
   mov 	DI, 81h     		; ES:DI-> ������ ������ � PSP
   mov 	SI,	offset key   	; DS:SI-> ���� key
   mov 	AL,	' '				; ������ ������� �� ������ ������
repe   scasb   	; ��������� ����� ���� �������
                ; AL - (ES:DI) -> ����� ����������
                ; ��������� ���� �������� �����
   dec 	DI          		; DI-> �� ������ ������ ����� ��������
	mov 	CX, 4       		; ��������� ����� �������
repe   cmpsb   	; ���������� ��������� ����� � ���������
                ; (DS:SI)-(ES:DI) -> ����� ����������
	jne 	check_install 		; ����������� ������� - ��������� ����������
	inc 	flag_off
; ��������, �� ����������� �� ��� ��� ���������
check_install:
	mov 	AX,	0C800h   		; AH=0C8h ����� �������� C8h
								; AL=00h -���� ������ ��������� ��������
	int 	2Fh        		; �������������� ����������
	cmp 	AL,	0FFh
	jne  	?cont1 				; ���������� AL=0FFh ���� �����������
;----------------------------------------------------------------------------
   jmp		already_ins 
?cont1:
	cmp 	flag_off,1
   jne  	?cont2
;----------------------------------------------------------------------------
   jmp	 	xm_stranno
?cont2:
	mov 	AX,	352Fh	   		;   �������� ������
	int 	21h					;	����������  2Fh
	mov 	word ptr int_2Fh_vector,	BX    	;   ES:BX - ������
   mov 	word ptr int_2Fh_vector+2,	ES  	;
	mov 	DX,	offset int_2Fh	;   �������� �������� ����� ����� � �����
								;   ���������� �� DX
	mov 	AX,	252Fh			;   ������� ��������� ����������
								;   �������� ������ 2Fh
   int 	21h 				; 	AL - ����� ������. 
								;	DS:DX - ��-�� ��������� ��������� ����.
;============================================================================
;============================================================================
	mov 	AX,	3521h         	;   �������� ������ ����������  21h
	int 	21h						;   ���������� �
	mov 	word ptr old_21h,	BX	;   ES:BX - ������
	mov 	word ptr old_21h+2,	ES	;
	mov 	DX,		offset new_21h	;   �������� �������� ����� ����� � �����
                             ;   ���������� �� DX
	mov 	AX,	2521h	;   ������� ��������� ���������� �������� ������ 21h
	int 	21h	;   AL - ����� ������. DS:DX - ��-�� ��������� ���. ����.
;
;============================================================================
	mov 	DX,	offset msg1			; ��������� �� ���������
	call	print
;============================================================================
;============================================================================
	mov DX,	offset	begin		;   �������� ��������� ...
   int 27h                	;   ... ����������� � �����
;============================================================================
already_ins:
		cmp 	flag_off,	1 	; ������ �� �������� ����������?
		je  	uninstall      ; ��, �� ��������
      lea 	DX,msg          ; ����� �� ����� ���������: already installed!
      call    print
      int 		20h
; ------------------ �������� -----------------------------------------------
 uninstall:
        mov AX,0C801h  ; AH=0C8h ����� ����. C8h, ���������� 01h-��������
        int 2Fh             ; �������������� ����������
        cmp AL,0F0h
        je  not_sucsess
        cmp AL,0Fh
        jne not_sucsess
        mov DX,offset msg2  ; ��������� � ��������
        call    print
        int 20h
not_sucsess:
        mov DX,offset msg3  ; ���������, ��� �������� ����������
        call    print
        int 20h
xm_stranno:
        mov DX,offset msg4  ; ���������, ��������� ���, � ������������
        call    print       ; ���� ������� ��������
        int 20h
;----------------------------------------------------------------------------
key        DB  '/off'
flag_off    DB  0
msg        DB  'already '
msg1       DB  'installed',13,10,'$'
msg4       DB  'just '
msg3       DB  'not '
msg2       DB  'uninstalled',13,10,'$'
;============================================================================
PRINT       PROC NEAR
    MOV AH,09H
    INT 21H
    RET
PRINT       ENDP
;============================================================================
;
code_seg ends
         end start
