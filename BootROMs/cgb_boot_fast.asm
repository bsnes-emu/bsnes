; Sameboy CGB bootstrap ROM
; Todo: use friendly names for HW registers instead of magic numbers
; Todo: add support for games that assume DMG boot logo (Such as X), like the
;       original boot ROM.
SECTION "BootCode", ROM0[$0]
Start:
; Init stack pointer
  ld sp, $fffe

; Select RAM bank
  ld a, 2
  ldh [$70], a
  xor a
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

  ld hl, $FF30
; Init waveform
  xor a
  ld c, $10
.waveformLoop
  ldi [hl], a
  cpl
  dec c
  jr nz, .waveformLoop

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

  ; Clear Palettes
  ld c, 64
  ld hl, BgPalettes
  ld a, $FF
.clearPalettesLoop:
  ldi [hl], a
  dec c
  jr nz, .clearPalettesLoop

  ld hl, BgPalettes
  ld d, 64 ; Length of write
  ld e, 0 ; Index of write
  call LoadBGPalettes

  ; Turn on LCD
  ld a, $91
  ldh [$40], a

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
  db \1 * 8, \2 * 8, \3 *8
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
  palette_comb 3, 3, 11
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
  palette_comb 27, 0, 14
  palette_comb 27, 4, 15
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
  dw $1B77, $0AD2, $25E9, $1545 ; DMG LCD

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


Preboot:
  call ClearVRAMViaHDMA
  ; Select the first bank
  xor a
  ldh [$4F], a
  call ClearVRAMViaHDMA

  ld a, [$143]
  bit 7, a
  jr nz, .cgbGame

  call EmulateDMG

.cgbGame
  ldh [$4C], a ; One day, I will know what this switch does and how it differs from FF6C
  ld a, $11
  ret

EmulateDMG:
  ld a, 1
  ldh [$6C], a ; DMG Emulation
  call GetPaletteIndex
  bit 7, a
  call nz, LoadDMGTilemap
  and $7F
  call WaitFrame
  call LoadPalettesFromIndex
  ld a, 4
  ret

GetPaletteIndex:
  ld a, [$14B] ; Old Licensee
  cp $33
  jr z, .newLicensee
  cp 1 ; Nintendo
  jr nz, .notNintendo
  jr .doChecksum
.newLicensee
  ld a, [$144]
  cp "0"
  jr nz, .notNintendo
  ld a, [$145]
  cp "1"
  jr nz, .notNintendo

.doChecksum
  ld hl, $134
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

SECTION "ROMMax", ROM0[$900]
  ; Prevent us from overflowing
  ds 1

SECTION "RAM", WRAM0[$C000]
BgPalettes:
  ds 8 * 4 * 2