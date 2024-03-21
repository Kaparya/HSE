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
	
    in      AL,60h      ; ������ scan-code
	s1:
		cmp     AL,13h      ; ��� ����-���  "R" ?
		jne s2
		cmp CS:flag[0],0
		jne s2 
		mov DL,0DAh        ; ������� ������ ������
		mov CS:flag[0],1
		jmp hotkey
	s2:
		cmp     AL,14h      ; ��� ����-��� "T" ?
		jne s3
		cmp CS:flag[1],0
		jne s3
		mov DL,0C4h          ; ������� ������ "-"
		mov CS:flag[1],1
		jmp hotkey 
	
	s3:
		cmp     AL,15h      ; ��� ����-��� "Y" ?
		jne s4
		cmp CS:flag[2],0
		jne s4
		mov DL,0BFh         ; ������� ������ ������
		mov CS:flag[2],1
		jmp hotkey 
		 
	s4:
		cmp     AL,21h      ; ��� ����-��� "F" ?
		jne s5
		cmp CS:flag[3],0
		jne s5
		mov DL,0B3h        
		mov CS:flag[3],1
		jmp hotkey 
		 
	s5:
		cmp     AL,22h      ; ��� ����-��� "G" ?
		jne s6
		cmp CS:flag[4],0
		jne s6
		mov DL,0B0h
		mov CS:flag[4],1
		jmp hotkey 
		
		 
	s6:
		cmp     AL,23h      ; ��� ����-��� "H" ?
		jne s7
		cmp CS:flag[5],0
		jne s7
		mov DL,0B3h
		mov CS:flag[5],1
		jmp hotkey 
		
	s7:
		cmp     AL,2Fh      ; ��� ����-��� "V" ?
		jne s8
		cmp CS:flag[6],0
		jne s8
		mov DL,0C0h
		mov CS:flag[6],1
		jmp hotkey 
		
	s8:
		cmp     AL,30h      ; ��� ����-��� "B" ?
		jne s9
		cmp CS:flag[7],0
		jne s9
		mov DL,0C4h
		mov CS:flag[7],1
		jmp hotkey 
		
	s9:
		cmp     AL,31h      ; ��� ����-��� "N" ?
		jne next
		cmp CS:flag[8],0
		jne next
		mov DL,0D9h
		mov CS:flag[8],1
		jmp hotkey 
		
	next:	
		pop     AX          ; No. ����������� AX
		popf
		jmp     dword ptr CS:[old_09h]  ; � ��������� ���������� ��� ��������
hotkey:
    sti                 ; �� ����� ������ �������
    in      AL,61h      ; ������ ���������� ����� B
    or      AL,80h      ; ��������� ������� ���
	
	mov AH,02h
	int 21h
    out     61h,AL      ; � ������ � ���� B.
    and     AL,7Fh      ; ����� �������� ������ ����������,
    out     61h,AL      ; ������� ������� ��� ����� B.
	jmp prodolzhenie
;

;---------------------------------------------------------------------------
prodolzhenie:
    cli
    mov     AL, 20h      ; ������
    out     20h,AL       ; ������ EOI
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
mov AX,0C700h ; AH=OC7h ����� �������� C7h
; AL=OOh -���� ������ ��������� ��������
int 2Fh ; �������������� ���������� 

cmp AL, 0
jz not_installed ; ���������� AL=0 ���� �� �����������
lea DX,msg 
call print 
int 20h 
msg DB 'Already installed',13,10,'$'
not_installed:

mov AX,352Fh ; �������� ������
int 21h ; ���������� 2Fh 
mov word ptr int_2Fh_vector,BX ; ES:BX - ������
mov word ptr int_2Fh_vector+2,ES 
mov DX,offset int_2Fh ; �������� �������� ����� ����� � ����� 
; ���������� �� DX
mov AX,252Fh ; ������� ��������� ���������� 
; �������� ������ 2Fh
int 21h ; AL - ����� ������. DS:DX - ��������� ��������� ��������� ����.
mov AX,3509h ; �������� ������
int 21h ; ���������� 09h
mov word ptr old_09h,BX ; ES:BX - ������
mov word ptr old_09h+2,ES ;
mov DX,offset new_09h ; �������� �������� ����� ����� � �����
; ���������� �� DX
mov AX,2509h ; ������� ��������� ���������� 
; �������� ������ 09h
int 21h ; AL - ����� ������. DS:DX - ��������� ��������� ��������� ����.
;------------------------------------------------------------------------------
mov DX,offset begin ; �������� ��������� ...
int 27h ; ... ����������� � �����
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