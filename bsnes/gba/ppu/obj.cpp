//BG modes 0,1,2 = 0x10000-0x17fff
//BG modes 3,4,5 = 0x14000-0x17fff

//OAM = 1024 bytes (128 entries x 64-bits)

// 0- 7 = y
//    8 = scale
//    9 = scaleflag (0 = single-fold, 1 = double-angle)
//10-11 = mode (0 = normal, 1 = semi-transparent, 2 = obj window, 3 = prohibited)
//   12 = mosaic
//   13 = colormode (0 = 16 colors x 16 palettes, 1 = 256 colors x 1 palette)
//14-15 = shape (0 = square, 1 = horizontal, 2 = vertical, 3 = prohibited)

//00-08 = x
//09-11 = rotation/scaling parameter
//   12 = hflip
//   13 = vflip
//14-15 = size

//00-09 = character
//10-11 = priority
//12-15 = palette (16-color mode)

void PPU::render_obj() {
}
