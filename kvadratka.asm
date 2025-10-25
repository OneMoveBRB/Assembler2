: Discriminant

; b^2
PUSHR RBX
PUSHR RBX
MUL

; 4*a*c
PUSH 4
PUSHR RAX
PUSHR RCX
MUL
MUL

; D = b^2 - 4*a*c
SUB
; sqrt(D)
SQRT

RET


; MAIN
IN

PUSH 3
PUSH -174
PUSH -3285

POPR RCX
POPR RBX
POPR RAX

; sqrt(D)
CALL Discriminant

; First root x1
POPR  RCX
PUSHR RCX

; b
PUSHR RBX

; sqrt(D) - b
SUB

; 2*a
PUSH  2
PUSHR RAX
MUL

; x1 = (sqrt(D) - b) / (2*a)
DIV

OUT

POP

; Second root x2
; Push sqrt(D)
PUSHR RCX

PUSH -1
MUL

; b
PUSHR RBX

; - sqrt(D) - b
SUB

; 2*a
PUSH  2
PUSHR RAX
MUL

; x2 = (- sqrt(D) - b) / (2*a)
DIV

OUT

POP

HLT
