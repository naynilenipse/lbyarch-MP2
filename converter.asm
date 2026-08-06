section .data
divisor dq 255.0
section .text
bits 64
default rel

global imgCvtGrayIntToDouble

imgCvtGrayIntToDouble:
    mov r10d, ecx
    imul r10d, edx ;total = width * height
    xor r11d, r11d ;initialize i(counter) as 0
L1:
    cmp r11d, r10d ;i>=total
    jge END ;ends if it's reached total's value
    
    CVTSI2SD xmm0, dword [r8 + r11*4];putting input[i] ([r8+r11*8]) into xmm0 reg. converting int to double
    divsd xmm0, qword [divisor] ;dividing double with 255.0
    movsd [r9+r11*8],xmm0 ;putting the result into output[i]
    
    inc r11d
    jmp L1
END: 
    ret