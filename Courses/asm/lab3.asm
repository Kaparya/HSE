code_seg segment
ASSUME CS:CODE_SEG,DS:code_seg,ES:code_seg
org 100h

start:
    jmp begin


begin:
    mov AX, ES:[80h]

code_seg ends
end start