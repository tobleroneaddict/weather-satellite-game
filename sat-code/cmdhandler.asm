;--------------------------------------------------------
; File Created by SDCC : free open source ISO C Compiler
; Version 4.5.0 #15242 (Linux)
;--------------------------------------------------------
	.module cmdhandler
	
	.optsdcc -mz80 sdcccall(1)
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _main
	.globl _isr_nmi
	.globl _flag
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area _DATA
_flag::
	.ds 2
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area _INITIALIZED
;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area _DABS (ABS)
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area _HOME
	.area _GSINIT
	.area _GSFINAL
	.area _GSINIT
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area _HOME
	.area _HOME
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area _CODE
;sat-code/cmdhandler.c:13: void isr_nmi(void) __interrupt(1)
;	---------------------------------
; Function isr_nmi
; ---------------------------------
_isr_nmi::
	ei
	push	af
	push	bc
	push	de
	push	hl
	push	iy
;sat-code/cmdhandler.c:15: *(uint8_t*)0x3010 = 'N';
	ld	hl, #0x3010
	ld	(hl), #0x4e
;sat-code/cmdhandler.c:16: }
	pop	iy
	pop	hl
	pop	de
	pop	bc
	pop	af
	reti
;sat-code/cmdhandler.c:18: void main(void) {
;	---------------------------------
; Function main
; ---------------------------------
_main::
;sat-code/cmdhandler.c:19: flag = (float*)0x3000;
	ld	hl, #0x3000
	ld	(_flag), hl
;sat-code/cmdhandler.c:24: while (1) {
00102$:
;sat-code/cmdhandler.c:25: *flag = (int)meow;
	ld	hl, (_flag)
	xor	a, a
	ld	(hl), a
	inc	hl
	ld	(hl), a
	inc	hl
	ld	(hl), #0xf0
	inc	hl
	ld	(hl), #0x41
;sat-code/cmdhandler.c:27: }
	jr	00102$
	.area _CODE
	.area _INITIALIZER
	.area _CABS (ABS)
