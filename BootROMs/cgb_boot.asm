; Sameboy CGB bootstrap ROM
; Todo: use friendly names for HW registers instead of magic numbers
SECTION "BootCode", ROM0[$0]
Start:
; Init stack pointer
    ld sp, $fffe

    xor a
; Clear chosen input palette
    ldh [InputPalette], a
; Clear title checksum
    ldh [TitleChecksum], a
; Clear memory VRAM
    ld hl, $8000
    call ClearMemoryPage
    ld h, $d0
    call ClearMemoryPage

; Clear OAM
    ld hl, $fe00
    ld c, $a0
    xor a
.clearOAMLoop
    ldi [hl], a
    dec c
    jr nz, .clearOAMLoop

; Init Audio
    ld a, $80
    ldh [$26], a
    ldh [$11], a
    ld a, $f3
    ldh [$12], a
    ldh [$25], a
    ld a, $77
    ldh [$24], a
    
    call InitWaveform

; Init BG palette
    ld a, $fc
    ldh [$47], a

; Load logo from ROM.
; A nibble represents a 4-pixels line, 2 bytes represent a 4x4 tile, scaled to 8x8.
; Tiles are ordered left to right, top to bottom.
; These tiles are not used, but are required for DMG compatibility. This is done
; by the original CGB Boot ROM as well.
    ld de, $104 ; Logo start
    ld hl, $8010 ; This is where we load the tiles in VRAM

.loadLogoLoop
    ld a, [de] ; Read 2 rows
    ld b, a
    call DoubleBitsAndWriteRow
    call DoubleBitsAndWriteRow
    inc de
    ld a, e
    xor $34 ; End of logo
    jr nz, .loadLogoLoop
    call ReadTrademarkSymbol

; Clear the second VRAM bank
    ld a, 1
    ldh [$4F], a
    xor a
    ld hl, $8000
    call ClearMemoryPage

; Copy Sameboy Logo
    ld de, SameboyLogo
    ld hl, $8080
    ld c, (SameboyLogoEnd - SameboyLogo) / 2
.sameboyLogoLoop
    ld a, [de]
    ldi [hl], a
    inc hl
    inc de
    ld a, [de]
    ldi [hl], a
    inc hl
    inc de
    dec c
    jr nz, .sameboyLogoLoop

; Copy (unresized) ROM logo
    ld de, $104
    ld c, 6
.CGBROMLogoLoop
    push bc
    call ReadCGBLogoTile
    pop bc
    dec c
    jr nz, .CGBROMLogoLoop
    inc hl
    call ReadTrademarkSymbol

; Load Tilemap
    ld hl, $98C2
    ld b, 3
    ld a, 8

.tilemapLoop
    ld c, $10

.tilemapRowLoop

    ld [hl], a
    push af
    ; Switch to second VRAM Bank
    ld a, 1
    ldh [$4F], a
    ld a, 8
    ld [hl], a
    ; Switch to back first VRAM Bank
    xor a
    ldh [$4F], a
    pop af
    ldi [hl], a
    inc a
    dec c
    jr nz, .tilemapRowLoop
    ld de, $10
    add hl, de
    dec b
    jr nz, .tilemapLoop

    cp $38
    jr nz, .doneTilemap

    ld hl, $99a7
    ld b, 1
    ld c, 7
    jr .tilemapRowLoop
.doneTilemap

    ; Expand Palettes
    ld de, AnimationColors
    ld c, 8
    ld hl, BgPalettes
    xor a
.expandPalettesLoop:
IF !DEF(FAST)
    cpl
ENDC
    ; One white
    ldi [hl], a
    ldi [hl], a

IF DEF(FAST)
    ; 3 more whites
    ldi [hl], a
    ldi [hl], a
    ldi [hl], a
    ldi [hl], a
    ldi [hl], a
    ldi [hl], a
ELSE    
    ; The actual color
    ld a, [de]
    inc de
    ldi [hl], a
    ld a, [de]
    inc de
    ldi [hl], a

    xor a
    ; Two blacks
    ldi [hl], a
    ldi [hl], a
    ldi [hl], a
    ldi [hl], a
ENDC

    dec c
    jr nz, .expandPalettesLoop

    ld hl, BgPalettes
    ld d, 64 ; Length of write
    ld e, 0 ; Index of write
    call LoadBGPalettes

    ; Turn on LCD
    ld a, $91
    ldh [$40], a

IF !DEF(FAST)
    call DoIntroAnimation

; Wait ~0.75 seconds
    ld b, 45
    call WaitBFrames

    ; Play first sound
    ld a, $83
    call PlaySound
    ld b, 5
    call WaitBFrames
    ; Play second sound
    ld a, $c1
    call PlaySound

; Wait ~0.5 seconds
    ld a, 30
    ldh [WaitLoopCounter], a
    
.waitLoop
    call GetInputPaletteIndex
    call WaitFrame
    ld hl, WaitLoopCounter
    dec [hl]
    jr nz, .waitLoop
ELSE
    ld a, $c1
    call PlaySound
ENDC
    call Preboot

; Will be filled with NOPs

SECTION "BootGame", ROM0[$fe]
BootGame:
    ldh [$50], a

SECTION "MoreStuff", ROM0[$200]

; Game Palettes Data
TitleChecksums:
    db $00 ; Default
    db $88 ; ALLEY WAY
    db $16 ; YAKUMAN
    db $36 ; BASEBALL, (Game and Watch 2)
    db $D1 ; TENNIS
    db $DB ; TETRIS
    db $F2 ; QIX
    db $3C ; DR.MARIO
    db $8C ; RADARMISSION
    db $92 ; F1RACE
    db $3D ; YOSSY NO TAMAGO
    db $5C ;
    db $58 ; X
    db $C9 ; MARIOLAND2
    db $3E ; YOSSY NO COOKIE
    db $70 ; ZELDA
    db $1D ;
    db $59 ;
    db $69 ; TETRIS FLASH
    db $19 ; DONKEY KONG
    db $35 ; MARIO'S PICROSS
    db $A8 ;
    db $14 ; POKEMON RED, (GAMEBOYCAMERA G)
    db $AA ; POKEMON GREEN
    db $75 ; PICROSS 2
    db $95 ; YOSSY NO PANEPON
    db $99 ; KIRAKIRA KIDS
    db $34 ; GAMEBOY GALLERY
    db $6F ; POCKETCAMERA
    db $15 ;
    db $FF ; BALLOON KID
    db $97 ; KINGOFTHEZOO
    db $4B ; DMG FOOTBALL
    db $90 ; WORLD CUP
    db $17 ; OTHELLO
    db $10 ; SUPER RC PRO-AM
    db $39 ; DYNABLASTER
    db $F7 ; BOY AND BLOB GB2
    db $F6 ; MEGAMAN
    db $A2 ; STAR WARS-NOA
    db $49 ;
    db $4E ; WAVERACE
    db $43 | $80 ;
    db $68 ; LOLO2
    db $E0 ; YOSHI'S COOKIE
    db $8B ; MYSTIC QUEST
    db $F0 ;
    db $CE ; TOPRANKINGTENNIS
    db $0C ; MANSELL
    db $29 ; MEGAMAN3
    db $E8 ; SPACE INVADERS
    db $B7 ; GAME&WATCH
    db $86 ; DONKEYKONGLAND95
    db $9A ; ASTEROIDS/MISCMD
    db $52 ; STREET FIGHTER 2
    db $01 ; DEFENDER/JOUST
    db $9D ; KILLERINSTINCT95
    db $71 ; TETRIS BLAST
    db $9C ; PINOCCHIO
    db $BD ;
    db $5D ; BA.TOSHINDEN
    db $6D ; NETTOU KOF 95
    db $67 ;
    db $3F ; TETRIS PLUS
    db $6B ; DONKEYKONGLAND 3
; For these games, the 4th letter is taken into account
FirstChecksumWithDuplicate:
    ; Let's play hangman!
    db $B3 ; ???[B]????????
    db $46 ; SUP[E]R MARIOLAND
    db $28 ; GOL[F]
    db $A5 ; SOL[A]RSTRIKER
    db $C6 ; GBW[A]RS
    db $D3 ; KAE[R]UNOTAMENI
    db $27 ; ???[B]????????
    db $61 ; POK[E]MON BLUE
    db $18 ; DON[K]EYKONGLAND
    db $66 ; GAM[E]BOY GALLERY2
    db $6A ; DON[K]EYKONGLAND 2
    db $BF ; KID[ ]ICARUS
    db $0D ; TET[R]IS2
    db $F4 ; ???[-]????????
    db $B3 ; MOG[U]RANYA
    db $46 ; ???[R]????????
    db $28 ; GAL[A]GA&GALAXIAN
    db $A5 ; BT2[R]AGNAROKWORLD
    db $C6 ; KEN[ ]GRIFFEY JR
    db $D3 ; ???[I]????????
    db $27 ; MAG[N]ETIC SOCCER
    db $61 ; VEG[A]S STAKES
    db $18 ; ???[I]????????
    db $66 ; MIL[L]I/CENTI/PEDE
    db $6A ; MAR[I]O & YOSHI
    db $BF ; SOC[C]ER
    db $0D ; POK[E]BOM
    db $F4 ; G&W[ ]GALLERY
    db $B3 ; TET[R]IS ATTACK
ChecksumsEnd:

PalettePerChecksum:
; | $80 means game requires DMG boot tilemap
    db 0 	; Default Palette
    db 4 	; ALLEY WAY
    db 5 	; YAKUMAN
    db 35 	; BASEBALL, (Game and Watch 2)
    db 34 	; TENNIS
    db 3 	; TETRIS
    db 31 	; QIX
    db 15 	; DR.MARIO
    db 10 	; RADARMISSION
    db 5 	; F1RACE
    db 19 	; YOSSY NO TAMAGO
    db 36 	;
    db 7 | $80 ; X
    db 37 	; MARIOLAND2
    db 30 	; YOSSY NO COOKIE
    db 44 	; ZELDA
    db 21 	;
    db 32 	;
    db 31 	; TETRIS FLASH
    db 20 	; DONKEY KONG
    db 5 	; MARIO'S PICROSS
    db 33 	;
    db 13 	; POKEMON RED, (GAMEBOYCAMERA G)
    db 14 	; POKEMON GREEN
    db 5 	; PICROSS 2
    db 29 	; YOSSY NO PANEPON
    db 5 	; KIRAKIRA KIDS
    db 18 	; GAMEBOY GALLERY
    db 9 	; POCKETCAMERA
    db 3 	;
    db 2 	; BALLOON KID
    db 26 	; KINGOFTHEZOO
    db 25 	; DMG FOOTBALL
    db 25 	; WORLD CUP
    db 41 	; OTHELLO
    db 42 	; SUPER RC PRO-AM
    db 26 	; DYNABLASTER
    db 45 	; BOY AND BLOB GB2
    db 42 	; MEGAMAN
    db 45 	; STAR WARS-NOA
    db 36 	;
    db 38 	; WAVERACE
    db 26 	;
    db 42 	; LOLO2
    db 30 	; YOSHI'S COOKIE
    db 41 	; MYSTIC QUEST
    db 34 	;
    db 34 	; TOPRANKINGTENNIS
    db 5 	; MANSELL
    db 42 	; MEGAMAN3
    db 6 	; SPACE INVADERS
    db 5 	; GAME&WATCH
    db 33 	; DONKEYKONGLAND95
    db 25 	; ASTEROIDS/MISCMD
    db 42 	; STREET FIGHTER 2
    db 42 	; DEFENDER/JOUST
    db 40 	; KILLERINSTINCT95
    db 2 	; TETRIS BLAST
    db 16 	; PINOCCHIO
    db 25 	;
    db 42 	; BA.TOSHINDEN
    db 42 	; NETTOU KOF 95
    db 5 	;
    db 0 	; TETRIS PLUS
    db 39 	; DONKEYKONGLAND 3
    db 36 	;
    db 22 	; SUPER MARIOLAND
    db 25 	; GOLF
    db 6 	; SOLARSTRIKER
    db 32 	; GBWARS
    db 12 	; KAERUNOTAMENI
    db 36 	;
    db 11 	; POKEMON BLUE
    db 39 	; DONKEYKONGLAND
    db 18 	; GAMEBOY GALLERY2
    db 39 	; DONKEYKONGLAND 2
    db 24 	; KID ICARUS
    db 31 	; TETRIS2
    db 50 	;
    db 17 	; MOGURANYA
    db 46 	;
    db 6 	; GALAGA&GALAXIAN
    db 27 	; BT2RAGNAROKWORLD
    db 0 	; KEN GRIFFEY JR
    db 47 	;
    db 41 	; MAGNETIC SOCCER
    db 41 	; VEGAS STAKES
    db 0 	;
    db 0 	; MILLI/CENTI/PEDE
    db 19 	; MARIO & YOSHI
    db 34 	; SOCCER
    db 23 	; POKEBOM
    db 18 	; G&W GALLERY
    db 29 	; TETRIS ATTACK

Dups4thLetterArray:
    db "BEFAARBEKEK R-URAR INAILICE R"

; We assume the last three arrays fit in the same $100 byte page!

PaletteCombinations:
palette_comb: MACRO ; Obj0, Obj1, Bg
    db (\1) * 8, (\2) * 8, (\3) *8
ENDM
raw_palette_comb: MACRO ; Obj0, Obj1, Bg
    db (\1) * 2, (\2) * 2, (\3) * 2
ENDM
    palette_comb 4, 4, 29
    palette_comb 18, 18, 18
    palette_comb 20, 20, 20
    palette_comb 24, 24, 24
    palette_comb 9, 9, 9
    palette_comb 0, 0, 0
    palette_comb 27, 27, 27
    palette_comb 5, 5, 5
    palette_comb 12, 12, 12
    palette_comb 26, 26, 26
    palette_comb 16, 8, 8
    palette_comb 4, 28, 28
    palette_comb 4, 2, 2
    palette_comb 3, 4, 4
    palette_comb 4, 29, 29
    palette_comb 28, 4, 28
    palette_comb 2, 17, 2
    palette_comb 16, 16, 8
    palette_comb 4, 4, 7
    palette_comb 4, 4, 18
    palette_comb 4, 4, 20
    palette_comb 19, 19, 9
    raw_palette_comb 4 * 4 - 1, 4 * 4 - 1, 11 * 4
    palette_comb 17, 17, 2
    palette_comb 4, 4, 2
    palette_comb 4, 4, 3
    palette_comb 28, 28, 0
    palette_comb 3, 3, 0
    palette_comb 0, 0, 1
    palette_comb 18, 22, 18
    palette_comb 20, 22, 20
    palette_comb 24, 22, 24
    palette_comb 16, 22, 8
    palette_comb 17, 4, 13
    raw_palette_comb 28 * 4 - 1, 0 * 4, 14 * 4
    raw_palette_comb 28 * 4 - 1, 4 * 4, 15 * 4
    palette_comb 19, 22, 9
    palette_comb 16, 28, 10
    palette_comb 4, 23, 28
    palette_comb 17, 22, 2
    palette_comb 4, 0, 2
    palette_comb 4, 28, 3
    palette_comb 28, 3, 0
    palette_comb 3, 28, 4
    palette_comb 21, 28, 4
    palette_comb 3, 28, 0
    palette_comb 25, 3, 28
    palette_comb 0, 28, 8
    palette_comb 4, 3, 28
    palette_comb 28, 3, 6
    palette_comb 4, 28, 29
    ; Sameboy "Exclusives"
    palette_comb 30, 30, 30 ; CGA
    palette_comb 31, 31, 31 ; DMG LCD
    palette_comb 28, 4, 1
    palette_comb 0, 0, 2

Palettes:
    dw $7FFF, $32BF, $00D0, $0000
    dw $639F, $4279, $15B0, $04CB
    dw $7FFF, $6E31, $454A, $0000
    dw $7FFF, $1BEF, $0200, $0000
    dw $7FFF, $421F, $1CF2, $0000
    dw $7FFF, $5294, $294A, $0000
    dw $7FFF, $03FF, $012F, $0000
    dw $7FFF, $03EF, $01D6, $0000
    dw $7FFF, $42B5, $3DC8, $0000
    dw $7E74, $03FF, $0180, $0000
    dw $67FF, $77AC, $1A13, $2D6B
    dw $7ED6, $4BFF, $2175, $0000
    dw $53FF, $4A5F, $7E52, $0000
    dw $4FFF, $7ED2, $3A4C, $1CE0
    dw $03ED, $7FFF, $255F, $0000
    dw $036A, $021F, $03FF, $7FFF
    dw $7FFF, $01DF, $0112, $0000
    dw $231F, $035F, $00F2, $0009
    dw $7FFF, $03EA, $011F, $0000
    dw $299F, $001A, $000C, $0000
    dw $7FFF, $027F, $001F, $0000
    dw $7FFF, $03E0, $0206, $0120
    dw $7FFF, $7EEB, $001F, $7C00
    dw $7FFF, $3FFF, $7E00, $001F
    dw $7FFF, $03FF, $001F, $0000
    dw $03FF, $001F, $000C, $0000
    dw $7FFF, $033F, $0193, $0000
    dw $0000, $4200, $037F, $7FFF
    dw $7FFF, $7E8C, $7C00, $0000
    dw $7FFF, $1BEF, $6180, $0000
    ; Sameboy "Exclusives"
    dw $7FFF, $7FEA, $7D5F, $0000 ; CGA 1
    dw $4778, $3290, $1D87, $0861 ; DMG LCD

KeyCombinationPalettes
    db 1 ; Right
    db 48 ; Left
    db 5 ; Up
    db 8 ; Down
    db 0 ; Right + A
    db 40 ; Left + A
    db 43 ; Up + A
    db 3 ; Down + A
    db 6 ; Right + B
    db 7 ; Left + B
    db 28 ; Up + B
    db 49 ; Down + B
    ; Sameboy "Exclusives"
    db 51 ; Right + A + B
    db 52 ; Left + A + B
    db 53 ; Up + A + B
    db 54 ; Down + A + B

TrademarkSymbol:
    db $3c,$42,$b9,$a5,$b9,$a5,$42,$3c

SameboyLogo:
    incbin "SameboyLogo.1bpp"
SameboyLogoEnd:

AnimationColors:
    dw $7FFF ; White
    dw $774F ; Cyan
    dw $22C7 ; Green
    dw $039F ; Yellow
    dw $017D ; Orange
    dw $241D ; Red
    dw $6D38 ; Purple
    dw $7102 ; Blue
AnimationColorsEnd:

DMGPalettes:
    dw $7FFF, $32BF, $00D0, $0000

; Helper Functions
DoubleBitsAndWriteRow:
; Double the most significant 4 bits, b is shifted by 4
    ld a, 4
    ld c, 0
.doubleCurrentBit
    sla b
    push af
    rl c
    pop af
    rl c
    dec a
    jr nz, .doubleCurrentBit
    ld a, c
; Write as two rows
    ldi [hl], a
    inc hl
    ldi [hl], a
    inc hl
    ret

WaitFrame:
    push hl
    ld hl, $FF0F
    res 0, [hl]
.wait
    bit 0, [hl]
    jr z, .wait
    pop hl
    ret

WaitBFrames:
    call GetInputPaletteIndex
    call WaitFrame
    dec b
    jr nz, WaitBFrames
    ret

PlaySound:
    ldh [$13], a
    ld a, $87
    ldh [$14], a
    ret

; Clear from HL to HL | 0x2000
ClearMemoryPage:
    ldi [hl], a
    bit 5, h
    jr z, ClearMemoryPage
    ret

; c = $f0 for even lines, $f for odd lines.
ReadTileLine:
    ld a, [de]
    and c
    ld b, a
    inc e
    inc e
    ld a, [de]
    dec e
    dec e
    and c
    swap a
    or b
    bit 0, c
    jr z, .dontSwap
    swap a
.dontSwap
    inc hl
    ldi [hl], a
    ret


ReadCGBLogoHalfTile:
    ld c, $f0
    call ReadTileLine
    ld c, $f
    call ReadTileLine
    inc e
    ld c, $f0
    call ReadTileLine
    ld c, $f
    call ReadTileLine
    inc e
    ret

ReadCGBLogoTile:
    call ReadCGBLogoHalfTile
    ld a, e
    add a, 22
    ld e, a
    call ReadCGBLogoHalfTile
    ld a, e
    sub a, 22
    ld e, a
    ret


ReadTrademarkSymbol:
    ld de, TrademarkSymbol
    ld c,$08
.loadTrademarkSymbolLoop:
    ld a,[de]
    inc de
    ldi [hl],a
    inc hl
    dec c
    jr nz, .loadTrademarkSymbolLoop
    ret

LoadObjPalettes:
    ld c, $6A
    jr LoadPalettes

LoadBGPalettes:
    ld c, $68

LoadPalettes:
    ld a, $80
    or e
    ld [c], a
    inc c
.loop
    ld a, [hli]
    ld [c], a
    dec d
    jr nz, .loop
    ret


AdvanceIntroAnimation:
    ld hl, $98C0
    ld c, 3 ; Row count
.loop
    ld a, [hl]
    cp $F ; Already blue
    jr z, .nextTile
    inc a
    ld [hl], a
    and $7
    cp $1 ; Changed a white tile, go to next line
    jr z, .nextLine
.nextTile
    inc hl
    jr .loop
.nextLine
    ld a, l
    or $1F
    ld l, a
    inc hl
    dec c
    ret z
    jr .loop

DoIntroAnimation:
    ; Animate the intro
    ld a, 1
    ldh [$4F], a
    ld d, 26
.animationLoop
    ld b, 2
    call WaitBFrames
    call AdvanceIntroAnimation
    dec d
    jr nz, .animationLoop
    ret

Preboot:
IF !DEF(FAST)
    call FadeOut
ENDC
    call ClearVRAMViaHDMA
    ; Select the first bank
    xor a
    ldh [$4F], a
    cpl
    ldh [$00], a
    call ClearVRAMViaHDMA
    
    ; Final values for CGB mode
    ld de, $ff56
    ld l, $0d
    
    ld a, [$143]
    bit 7, a
    call z, EmulateDMG
    ldh [$4C], a
    ldh a, [TitleChecksum]
    ld b, a
    
    ldh a, [InputPalette]
    and a
    jr nz, .emulateDMGForCGBGame
IF DEF(AGB)
    ; Set registers to match the original AGB-CGB boot
    ; AF = $1100, C = 0
    xor a
    ld c, a
    add a, $11
    ld h, c
    ld b, 1
ELSE
    ; Set registers to match the original CGB boot
    ; AF = $1180, C = 0
    xor a
    ld c, a
    ld a, $11
    ld h, c
    ; B is set to the title checksum
ENDC
    ret

.emulateDMGForCGBGame
    call EmulateDMG
    ldh [$4C], a
    ld a, $1;
    ret

EmulateDMG:
    ld a, 1
    ldh [$6C], a ; DMG Emulation
    call GetPaletteIndex
    bit 7, a
    call nz, LoadDMGTilemap
    and $7F
    ld b, a
    ldh a, [InputPalette]
    and a
    jr z, .nothingDown
    ld hl, KeyCombinationPalettes - 1 ; Return value is 1-based, 0 means nothing down
    ld c ,a
    ld b, 0
    add hl, bc
    ld a, [hl]
    jr .paletteFromKeys
.nothingDown
    ld a, b
.paletteFromKeys
    call WaitFrame
    call LoadPalettesFromIndex
    ld a, 4
    ; Set the final values for DMG mode
    ld d, 0
    ld e, $8
    ld l, $7c
    ret

GetPaletteIndex:
    ld hl, $14B
    ld a, [hl] ; Old Licensee
    cp $33
    jr z, .newLicensee
    cp 1 ; Nintendo
    jr nz, .notNintendo
    jr .doChecksum
.newLicensee
    ld l, $44
    ld a, [hli]
    cp "0"
    jr nz, .notNintendo
    ld a, [hl]
    cp "1"
    jr nz, .notNintendo

.doChecksum
    ld l, $34
    ld c, $10
    ld b, 0

.checksumLoop
    ld a, [hli]
    add b
    ld b, a
    dec c
    jr nz, .checksumLoop

    ; c = 0
    ld hl, TitleChecksums

.searchLoop
    ld a, l
    cp ChecksumsEnd & $FF
    jr z, .notNintendo
    ld a, [hli]
    cp b
    jr nz, .searchLoop

    ; We might have a match, Do duplicate/4th letter check
    ld a, l
    sub FirstChecksumWithDuplicate - TitleChecksums
    jr c, .match ; Does not have a duplicate, must be a match!
    ; Has a duplicate; check 4th letter
    push hl
    ld a, l
    add Dups4thLetterArray - FirstChecksumWithDuplicate - 1 ; -1 since hl was incremented
    ld l, a
    ld a, [hl]
    pop hl
    ld c, a
    ld a, [$134 + 3] ; Get 4th letter
    cp c
    jr nz, .searchLoop ; Not a match, continue

.match
    ld a, l
    add PalettePerChecksum - TitleChecksums - 1; -1 since hl was incremented
    ld l, a
    ld a, b
    ldh [TitleChecksum], a
    ld a, [hl]
    ret

.notNintendo
    xor a
    ret

LoadPalettesFromIndex: ; a = index of combination
    ld b, a
    ; Multiply by 3
    add b
    add b

    ld hl, PaletteCombinations
    ld b, 0
    ld c, a
    add hl, bc

    ; Obj Palettes
    ld e, 0
.loadObjPalette
    ld a, [hli]
    push hl
    ld hl, Palettes
    ld b, 0
    ld c, a
    add hl, bc
    ld d, 8
    call LoadObjPalettes
    pop hl
    bit 3, e
    jr nz, .loadBGPalette
    ld e, 8
    jr .loadObjPalette
.loadBGPalette
    ;BG Palette
    ld a, [hli]
    ld hl, Palettes
    ld b, 0
    ld c, a
    add hl, bc
    ld d, 8
    ld e, 0
    call LoadBGPalettes
    ret

BrightenColor:
    ld a, [hli]
    ld e, a
    ld a, [hld]
    ld d, a
    ; RGB(1,1,1)
    ld bc, $421

    ; Is blue maxed?
    ld a, e
    and $1F
    cp $1F
    jr nz, .blueNotMaxed
    res 0, c
.blueNotMaxed

    ; Is green maxed?
    ld a, e
    and $E0
    cp $E0
    jr nz, .greenNotMaxed
    ld a, d
    and $3
    cp $3
    jr nz, .greenNotMaxed
    res 5, c
.greenNotMaxed

    ; Is red maxed?
    ld a, d
    and $7C
    cp $7C
    jr nz, .redNotMaxed
    res 2, b
.redNotMaxed

    ; Add de to bc
    push hl
    ld h, d
    ld l, e
    add hl, bc
    ld d, h
    ld e, l
    pop hl

    ld a, e
    ld [hli], a
    ld a, d
    ld [hli], a
    ret

FadeOut:
    ld b, 32 ; 32 times to fade
.fadeLoop
    ld c, 32 ; 32 colors to fade
    ld hl, BgPalettes
.frameLoop
    push bc
    call BrightenColor
    pop bc
    dec c
    jr nz, .frameLoop

    call WaitFrame
    call WaitFrame
    ld hl, BgPalettes
    ld d, 64 ; Length of write
    ld e, 0 ; Index of write
    call LoadBGPalettes
    dec b
    ret z
    jr .fadeLoop

ClearVRAMViaHDMA:
    ld hl, $FF51

    ; Src
    ld a, $D0
    ld [hli], a
    xor a
    ld [hli], a

    ; Dest
    ld a, $98
    ld [hli], a
    ld a, $A0
    ld [hli], a

    ; Do it
    ld a, $12
    ld [hli], a
    ret

GetInputPaletteIndex:
    ld a, $20 ; Select directions
    ldh [$00], a
    ldh a, [$00]
    cpl
    and $F
    ret z ; No direction keys pressed, no palette
    push bc
    ld c, 0

.directionLoop
    inc c
    rra
    jr nc, .directionLoop

    ; c = 1: Right, 2: Left, 3: Up, 4: Down

    ld a, $10 ; Select buttons
    ldh [$00], a
    ldh a, [$00]
    cpl
    rla
    rla
    and $C
    add c
    ld b, a
    ldh a, [InputPalette]
    ld c, a
    ld a, b
    ldh [InputPalette], a
    cp c
    pop bc
    ret z ; No change, don't load
    ; Slide into change Animation Palette

ChangeAnimationPalette:
    push af
    push hl
    push bc
    push de
    ld hl, KeyCombinationPalettes - 1 ; Input palettes are 1-based, 0 means nothing down
    ld c ,a
    ld b, 0
    add hl, bc
    ld a, [hl]
    ld b, a
    ; Multiply by 3
    add b
    add b

    ld hl, PaletteCombinations + 2; Background Palette
    ld b, 0
    ld c, a
    add hl, bc
    ld a, [hl]
    ld hl, Palettes + 1
    ld b, 0
    ld c, a
    add hl, bc
    ld a, [hld]
    cp $7F ; Is white color?
    jr nz, .isWhite
    inc hl
    inc hl
.isWhite
    push af
    ld a, [hli]
    push hl
    ld hl, BgPalettes ; First color, all palette
    call ReplaceColorInAllPalettes
    pop hl
    ldh [BgPalettes + 2], a ; Second color, first palette

    ld a, [hli]
    push hl
    ld hl, BgPalettes + 1 ; First color, all palette
    call ReplaceColorInAllPalettes
    pop hl
    ldh [BgPalettes + 3], a ; Second color, first palette
    pop af
    jr z, .isNotWhite
    inc hl
    inc hl
.isNotWhite
    ld a, [hli]
    ldh [BgPalettes + 7 * 8 + 2], a ; Second color, 7th palette
    ld a, [hli]
    ldh [BgPalettes + 7 * 8 + 3], a ; Second color, 7th palette
    ld a, [hli]
    ldh [BgPalettes + 4], a ; Third color, first palette
    ld a, [hl]
    ldh [BgPalettes + 5], a ; Third color, first palette

    call WaitFrame
    ld hl, BgPalettes
    ld d, 64 ; Length of write
    ld e, 0 ; Index of write
    call LoadBGPalettes
    ; Delay the wait loop while the user is selecting a palette
    ld a, 30
    ldh [WaitLoopCounter], a
    pop de
    pop bc
    pop hl
    pop af
    ret

ReplaceColorInAllPalettes:
    ld de, 8
    ld c, 8
.loop
    ld [hl], a
    add hl, de
    dec c
    jr nz, .loop
    ret
    
LoadDMGTilemap:
    push af
    call WaitFrame
    ld a,$19      ; Trademark symbol
    ld [$9910], a ; ... put in the superscript position
    ld hl,$992f   ; Bottom right corner of the logo
    ld c,$c       ; Tiles in a logo row
.tilemapLoop
    dec a
    jr z, .tilemapDone
    ldd [hl], a
    dec c
    jr nz, .tilemapLoop
    ld l,$0f ; Jump to top row
    jr .tilemapLoop
.tilemapDone
    pop af
    ret

InitWaveform:
    ld hl, $FF30
; Init waveform
    xor a
    ld c, $10
.waveformLoop
    ldi [hl], a
    cpl
    dec c
    jr nz, .waveformLoop
    ret

SECTION "ROMMax", ROM0[$900]
    ; Prevent us from overflowing
    ds 1

SECTION "HRAM", HRAM[$FF80]
TitleChecksum:
    ds 1
BgPalettes:
    ds 8 * 4 * 2
InputPalette:
    ds 1
WaitLoopCounter:
    ds 1