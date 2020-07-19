ideal

p386
p387
model flat
codeseg

locals

public lowlevel_rect_f_
public lowlevel_filled_rect_f_


; lowlevel_rect_f_
;    eax    = color
;    edx    = width
;    ebx    = y_inc
;    ecx    = lines
;    ebp+8  = p1
;    ebp+12 = p2
proc lowlevel_rect_f_ near
arg @@p1:dword, @@p2:dword

    push ebp
    mov ebp, esp
    push edi
    push esi

    mov ah, al            ; spread color byte out over all 32-bits
    shl eax, 8            ; so 4 pixels can be written horizontally
    mov al, ah            ; later on
    shl eax, 8
    mov al, ah

    mov edi, [@@p1]       ; left edge
    mov esi, [@@p2]       ; right edge

    ; al  = color to draw
    ; ebx = amount to inc both pointers by to move down 1 line
    ; ecx = number of lines vertically to draw

    test ecx, ecx
    jz @@draw_horiz1

@@draw_vert:
    mov [edi], al
    mov [esi], al
    add edi, ebx
    add esi, ebx
    dec ecx
    jnz @@draw_vert

@@draw_horiz1:
    ; edi is currently at (x1,y2) after previous drawing. this is the
    ; correct location to draw the bottom horizontal line, so we will
    ; draw it now
    mov ecx, edx          ; edx = number of pixels to draw
    mov ebx, ecx          ; ecx = number of dwords to draw
    shr ecx, 2            ; ebx = remaining number of pixels ( <= 3 )
    and ebx, 3

    rep stosd             ; draw the line
    mov ecx, ebx
    rep stosb

@@draw_horiz2:
    ; now draw top horizontal line
    mov edi, [@@p1]       ; reset edi to top horizontal line start position
    mov ecx, edx          ; edx = number of pixels to draw
    mov ebx, ecx          ; ecx = number of dwords to draw
    shr ecx, 2            ; ebx = remaining number of pixels ( <= 3 )
    and ebx, 3

    rep stosd             ; draw the line
    mov ecx, ebx
    rep stosb

    pop esi
    pop edi
    pop ebp
    ret 8

    endp

; lowlevel_filled_rect_f_
;    eax    = color
;    edx    = y_inc
;    ebx    = width
;    ecx    = lines
;    ebp+8  = dest
proc lowlevel_filled_rect_f_ near
arg @@dest:dword

    push ebp
    mov ebp, esp
    push edi
    push esi

    mov esi, ecx          ; get number of lines to be drawn
    test esi, esi         ; if there are no lines to draw, then return
    jz @@early_done

    mov ah, al            ; spread color byte out over all 32-bits
    shl eax, 8            ; so 4 pixels can be written horizontally
    mov al, ah            ; later on
    shl eax, 8
    mov al, ah

    mov edi, [@@dest]
    sub edx, ebx          ; edx = y_inc - width
                          ; (this is because rep stos will inc edi, so we
                          ; just need to y-inc by the remaining amount)

    ; *** WARNING: no stack/locals access after this! :) ***
    push ebp
    mov ebp, ebx          ; get width as number of dwords and remaining bytes
    and ebp, 3
    shr ebx, 2

    ; esi = loop counter of number of lines to draw
    ; edi = destination to draw at
    ; eax = color to draw (filled out over all 32-bits)
    ; edx = y_inc - width
    ; ebx = number of dwords to draw
    ; ebp = remaining number of pixels to draw after the dwords

    test ebp, ebp         ; if there are no remaining bytes after dwords
    jz @@draw_4           ; then we can do a slightly more optimized draw

@@draw_4_with_remainder:  ; scenario #1: draw dwords + remainder
    mov ecx, ebx
    rep stosd
    mov ecx, ebp
    rep stosb

    add edi, edx          ; move to start of next line
    dec esi               ; decrease loop counter (lines left to draw)
    jnz @@draw_4_with_remainder
    jmp @@done            ; done drawing, skip to return

@@draw_4:                 ; scenario #2: draw dwords only
    mov ecx, ebx
    rep stosd

    add edi, edx          ; move to start of next line
    dec esi               ; decrease loop counter (lines left to draw)
    jnz @@draw_4

@@done:
    pop ebp
@@early_done:
    pop esi
    pop edi
    pop ebp
    ret 4

    endp

end

