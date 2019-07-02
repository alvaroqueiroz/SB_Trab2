_printEAX:
        enter 0,0
        mov eax, [ebp+8]
        mov word [sum_out], 10
        mov ecx, sum_out
        inc ecx 

_printEAXloop:
        sub edx, edx
        mov ebx, 10
        div ebx

        cmp dl, 10
        add edx, 48 ; ascii for '0'

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