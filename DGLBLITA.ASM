ideal

p386
p387
model flat
codeseg

locals

public lowlevel_blit_4_
public lowlevel_blit_4r_
public lowlevel_blit_r_
public lowlevel_blit_sprite_4_
public lowlevel_blit_sprite_4r_
public lowlevel_blit_sprite_r_
public lowlevel_blit_sprite_8_
public lowlevel_blit_sprite_8r_

; lowlevel_blit_4_
;    eax    = width4
;    edx    = lines
;    ebx    = dest
;    ecx    = src
;    ebp+8  = dest_y_inc
;    ebp+12 = src_y_inc
proc lowlevel_blit_4_ near
arg @@dest_y_inc:dword, @@src_y_inc:dword

    push ebp
    mov ebp, esp
    push edi
    push esi

    mov edi, ebx             ; dest pixels
    mov esi, ecx             ; source pixels

    mov ebx, [@@src_y_inc]
    mov ebp, [@@dest_y_inc]  ; WARNING: no stack/args access after this!

    ; eax = number of 4-pixel runs (dwords)
    ; edx = line loop counter
    ; ebx = src_y_inc
    ; ebp = dest_y_inc

    test edx, edx            ; make sure there is >0 lines to draw
    jz @@done

@@draw_line:
    mov ecx, eax             ; draw all 4-pixel runs (dwords)
    rep movsd

    add esi, ebx             ; move to next line
    add edi, ebp
    dec edx                  ; decrease line loop counter
    jnz @@draw_line          ; keep going if there's more lines to draw

@@done:
    pop esi
    pop edi
    pop ebp
    ret 8

    endp

; lowlevel_blit_4r_
;    eax    = width4
;    edx    = lines
;    ebx    = remainder
;    ecx    = dest
;    ebp+8  = src
;    ebp+12 = dest_y_inc
;    ebp+16 = src_y_inc
proc lowlevel_blit_4r_ near
arg @@src:dword, @@dest_y_inc:dword, @@src_y_inc:dword

    push ebp
    mov ebp, esp
    push edi
    push esi

    mov edi, ecx             ; dest pixels
    mov esi, [@@src]         ; source pixels

    ; eax = number of 4-pixel runs (dwords)
    ; ebx = remaining number of pixels
    ; edx = line loop counter

    test edx, edx            ; make sure there is >0 lines to draw
    jz @@done

@@draw_line:
    mov ecx, eax             ; draw all 4-pixel runs (dwords)
    rep movsd
    mov ecx, ebx             ; draw remaining pixels ( <= 3 bytes )
    rep movsb

    add esi, [@@src_y_inc]   ; move to next line
    add edi, [@@dest_y_inc]
    dec edx                  ; decrease line loop counter
    jnz @@draw_line          ; keep going if there's more lines to draw

@@done:
    pop esi
    pop edi
    pop ebp
    ret 12

    endp

; lowlevel_blit_r_
;    eax    = width
;    edx    = lines
;    ebx    = dest
;    ecx    = src
;    ebp+8  = dest_y_inc
;    ebp+12 = src_y_inc
proc lowlevel_blit_r_ near
arg @@dest_y_inc:dword, @@src_y_inc:dword

    push ebp
    mov ebp, esp
    push edi
    push esi

    mov edi, ebx             ; dest pixels
    mov esi, ecx             ; source pixels

    ; eax = number of pixels to draw (bytes)
    ; edx = line loop counter

    test edx, edx            ; make sure there is >0 lines to draw
    jz @@done

@@draw_line:
    mov ecx, eax             ; draw pixels (bytes)
    rep movsb

    add esi, [@@src_y_inc]   ; move to next line
    add edi, [@@dest_y_inc]
    dec edx                  ; decrease line loop counter
    jnz @@draw_line          ; keep going if there's more lines to draw

@@done:
    pop esi
    pop edi
    pop ebp
    ret 8

    endp

; lowlevel_blit_sprite_4_
;    eax    = width4
;    edx    = lines
;    ebx    = dest
;    ecx    = src
;    ebp+8  = dest_y_inc
;    ebp+12 = src_y_inc
proc lowlevel_blit_sprite_4_ near
arg @@dest_y_inc:dword, @@src_y_inc:dword

    push ebp
    mov ebp, esp
    push edi
    push esi

    mov edi, ebx             ; dest pixels
    mov esi, ecx             ; source pixels

    ; eax = number of 4-pixel runs (dwords)
    ; edx = line loop counter

    test edx, edx            ; make sure there is >0 lines to be drawn
    jz @@done

@@draw_line:

@@start_4_run:
    mov ecx, eax             ; ecx = counter of 4-pixel runs left to draw
@@draw_px_0:
    mov bx, [esi+0]          ; load src pixel
    test bl, bl
    jz @@draw_px_1           ; if it is color 0, skip it
    mov [edi+0], bl          ; otherwise, draw it onto dest
@@draw_px_1:
    test bh, bh
    jz @@draw_px_2
    mov [edi+1], bh
@@draw_px_2:
    mov bx, [esi+2]
    test bl, bl
    jz @@draw_px_3
    mov [edi+2], bl
@@draw_px_3:
    test bh, bh
    jz @@end_4_run
    mov [edi+3], bh
@@end_4_run:
    add esi, 4               ; move src and dest up 4 pixels
    add edi, 4
    dec ecx                  ; decrease 4-pixel run loop counter
    jnz @@draw_px_0          ; if there are still more runs, draw them

@@end_line:
    add esi, [@@src_y_inc]   ; move src and dest to start of next line
    add edi, [@@dest_y_inc]
    dec edx                  ; decrease line loop counter
    jnz @@draw_line          ; keep going if there's more lines to draw

@@done:
    pop esi
    pop edi
    pop ebp
    ret 8

    endp

; lowlevel_blit_sprite_4r_
;    eax    = width4
;    edx    = lines
;    ebx    = dest
;    ecx    = src
;    ebp+8  = remainder
;    ebp+12 = dest_y_inc
;    ebp+16 = src_y_inc
proc lowlevel_blit_sprite_4r_ near
arg @@remainder:dword, @@dest_y_inc:dword, @@src_y_inc:dword

    push ebp
    mov ebp, esp
    push edi
    push esi

    mov edi, ebx             ; dest pixels
    mov esi, ecx             ; source pixels

    ; eax = number of 4-pixel runs (dwords)
    ; edx = line loop counter

    test edx, edx            ; make sure there is >0 lines to be drawn
    jz @@done

@@draw_line:

@@start_4_run:               ; draw 4-pixel runs first
    mov ecx, eax             ; ecx = counter of 4-pixel runs left to draw
@@draw_px_0:
    mov bx, [esi+0]          ; load src pixel
    test bl, bl
    jz @@draw_px_1           ; if it is color 0, skip it
    mov [edi+0], bl          ; otherwise, draw it onto dest
@@draw_px_1:
    test bh, bh
    jz @@draw_px_2
    mov [edi+1], bh
@@draw_px_2:
    mov bx, [esi+2]
    test bl, bl
    jz @@draw_px_3
    mov [edi+2], bl
@@draw_px_3:
    test bh, bh
    jz @@end_4_run
    mov [edi+3], bh
@@end_4_run:
    add esi, 4               ; move src and dest up 4 pixels
    add edi, 4
    dec ecx                  ; decrease 4-pixel run loop counter
    jnz @@draw_px_0          ; if there are still more runs, draw them

@@start_remainder_run:       ; now draw remaining pixels ( <= 3 pixels )
    mov ecx, [@@remainder]   ; ecx = counter of remaining pixels

@@draw_pixel:
    mov bl, [esi]            ; load pixel
    inc esi
    test bl, bl              ; if zero, skip to next pixel
    jz @@end_pixel
    mov [edi], bl            ; else, draw pixel
@@end_pixel:
    inc edi
    dec ecx
    jnz @@draw_pixel         ; keep drawing pixels while there's still more

@@end_line:
    add esi, [@@src_y_inc]   ; move src and dest to start of next line
    add edi, [@@dest_y_inc]
    dec edx                  ; decrease line loop counter
    jnz @@draw_line          ; keep going if there's more lines to draw

@@done:
    pop esi
    pop edi
    pop ebp
    ret 12

    endp

; lowlevel_blit_sprite_r_
;    eax    = width
;    edx    = lines
;    ebx    = dest
;    ecx    = src
;    ebp+8  = dest_y_inc
;    ebp+12 = src_y_inc
proc lowlevel_blit_sprite_r_ near
arg @@dest_y_inc:dword, @@src_y_inc:dword

    push ebp
    mov ebp, esp
    push edi
    push esi

    mov edi, ebx             ; dest pixels
    mov esi, ecx             ; source pixels

    ; eax = number of 4-pixel runs (dwords)
    ; edx = line loop counter

    test edx, edx            ; make sure there is >0 lines to be drawn
    jz @@done

@@draw_line:
    mov ecx, eax             ; ecx = counter of remaining pixels

@@draw_pixel:
    mov bl, [esi]            ; load pixel
    inc esi
    test bl, bl              ; if zero, skip to next pixel
    jz @@end_pixel
    mov [edi], bl            ; else, draw pixel
@@end_pixel:
    inc edi
    dec ecx
    jnz @@draw_pixel         ; loop while there's still pixels left

@@end_line:
    add esi, [@@src_y_inc]   ; move src and dest to start of next line
    add edi, [@@dest_y_inc]
    dec edx                  ; decrease line loop counter
    jnz @@draw_line          ; keep going if there's more lines to draw

@@done:
    pop esi
    pop edi
    pop ebp
    ret 8

    endp

; lowlevel_blit_sprite_8_
;    eax    = width8
;    edx    = lines
;    ebx    = dest
;    ecx    = src
;    ebp+8  = dest_y_inc
;    ebp+12 = src_y_inc
proc lowlevel_blit_sprite_8_ near
arg @@dest_y_inc:dword, @@src_y_inc:dword

    push ebp
    mov ebp, esp
    push edi
    push esi

    mov edi, ebx             ; dest pixels
    mov esi, ecx             ; source pixels

    ; eax = number of 8-pixel runs
    ; edx = line loop counter

    test edx, edx            ; make sure there is >0 lines to be drawn
    jz @@done

@@draw_line:
    mov ecx, eax             ; ecx = counter of 8-pixel runs left to draw
@@draw_px_0:
    mov bx, [esi+0]          ; load src pixel
    test bl, bl
    jz @@draw_px_1           ; if it is color 0, skip it
    mov [edi+0], bl          ; otherwise, draw it onto dest
@@draw_px_1:
    test bh, bh
    jz @@draw_px_2
    mov [edi+1], bh
@@draw_px_2:
    mov bx, [esi+2]
    test bl, bl
    jz @@draw_px_3
    mov [edi+2], bl
@@draw_px_3:
    test bh, bh
    jz @@draw_px_4
    mov [edi+3], bh
@@draw_px_4:
    mov bx, [esi+4]
    test bl, bl
    jz @@draw_px_5
    mov [edi+4], bl
@@draw_px_5:
    test bh, bh
    jz @@draw_px_6
    mov [edi+5], bh
@@draw_px_6:
    mov bx, [esi+6]
    test bl, bl
    jz @@draw_px_7
    mov [edi+6], bl
@@draw_px_7:
    test bh, bh
    jz @@end_8_run
    mov [edi+7], bh
@@end_8_run:
    add esi, 8               ; move src and dest up 8 pixels
    add edi, 8
    dec ecx                  ; decrease 8-pixel run loop counter
    jnz @@draw_px_0          ; if there are still more runs, draw them

@@end_line:
    add esi, [@@src_y_inc]   ; move src and dest to start of next line
    add edi, [@@dest_y_inc]
    dec edx                  ; decrease line loop counter
    jnz @@draw_line          ; keep going if there's more lines to draw

@@done:
    pop esi
    pop edi
    pop ebp
    ret 8

    endp

; lowlevel_blit_sprite_8r_
;    eax    = width8
;    edx    = lines
;    ebx    = dest
;    ecx    = src
;    ebp+8  = remainder
;    ebp+12 = dest_y_inc
;    ebp+16 = src_y_inc
proc lowlevel_blit_sprite_8r_ near
arg @@remainder:dword, @@dest_y_inc:dword, @@src_y_inc:dword

    push ebp
    mov ebp, esp
    push edi
    push esi

    mov edi, ebx             ; dest pixels
    mov esi, ecx             ; source pixels

    ; eax = number of 8-pixel runs
    ; edx = line loop counter

    test edx, edx            ; make sure there is >0 lines to be drawn
    jz @@done

@@draw_line:

@@start_8_run:               ; draw 8-pixel runs first
    mov ecx, eax             ; ecx = counter of 8-pixel runs left to draw
@@draw_px_0:
    mov bx, [esi+0]          ; load src pixel
    test bl, bl
    jz @@draw_px_1           ; if it is color 0, skip it
    mov [edi+0], bl          ; otherwise, draw it onto dest
@@draw_px_1:
    test bh, bh
    jz @@draw_px_2
    mov [edi+1], bh
@@draw_px_2:
    mov bx, [esi+2]
    test bl, bl
    jz @@draw_px_3
    mov [edi+2], bl
@@draw_px_3:
    test bh, bh
    jz @@draw_px_4
    mov [edi+3], bh
@@draw_px_4:
    mov bx, [esi+4]
    test bl, bl
    jz @@draw_px_5
    mov [edi+4], bl
@@draw_px_5:
    test bh, bh
    jz @@draw_px_6
    mov [edi+5], bh
@@draw_px_6:
    mov bx, [esi+6]
    test bl, bl
    jz @@draw_px_7
    mov [edi+6], bl
@@draw_px_7:
    test bh, bh
    jz @@end_8_run
    mov [edi+7], bh
@@end_8_run:
    add esi, 8               ; move src and dest up 8 pixels
    add edi, 8
    dec ecx                  ; decrease 8-pixel run loop counter
    jnz @@draw_px_0          ; if there are still more runs, draw them

@@start_remainder_run:       ; now draw remaining pixels ( <= 7 pixels )
    mov ecx, [@@remainder]   ; ecx = counter of remaining pixels

@@draw_pixel:
    mov bl, [esi]            ; load pixel
    inc esi
    test bl, bl              ; if zero, skip to next pixel
    jz @@end_pixel
    mov [edi], bl            ; else, draw pixel
@@end_pixel:
    inc edi
    dec ecx
    jnz @@draw_pixel         ; loop while there's still pixels left

@@end_line:
    add esi, [@@src_y_inc]   ; move src and dest to start of next line
    add edi, [@@dest_y_inc]
    dec edx                  ; decrease line loop counter
    jnz @@draw_line          ; keep going if there's more lines to draw

@@done:
    pop esi
    pop edi
    pop ebp
    ret 12

    endp

end

