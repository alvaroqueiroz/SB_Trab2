 
_printEAX:
        enter 0,0
        mov eax, [ebp+8]
        mov word [sum_out], 10
        mov ecx, sum_out
        inc ecx 

_printEAXloop:
        sub edx, edx
        mov ebx, 16
        div ebx

        cmp dl, 10
        jge _letra

        add edx, 48 ; ascii for '0'
_continue:

        mov [ecx], dl  
        inc ecx
    
        cmp eax, 0
        jne _printEAXloop

_printEAXloop2:
        mov eax, 4
        mov ebx, 1 ; ecx is already defined
        mov edx, 1
        int 80h

        dec ecx

        cmp ecx, sum_out
        jge _printEAXloop2

        leave
        ret

_letra:
        add edx, 55
        jmp _continue