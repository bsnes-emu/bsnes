/*
  $2101 : OAM settings
  sssnnbbb

  s: base sprite size
          small : large
     000:   8x8 : 16x16
     001:   8x8 : 32x32
     010:   8x8 : 64x64
     011: 16x16 : 32x32
     100: 16x16 : 64x64
     101: 32x32 : 64x64
     110: 16x32 : 32x64
     111: 16x32 : 32x32
     small/large is determined by oam size bit
  n: name selection (0-3)
  b: oam tiledata location (>>14) -- highest bit ignored
*/
void mmio_w2101(byte value) {
  ppu.oam_base_size    = (value >> 5);
  ppu.oam_name_sel     = (value >> 3) & 3;
  ppu.oam_tiledata_loc = (value & 3) << 14;
}

/*
  $2102/$2103 : OAM access address
  $2102: llllllll
  $2103: ???????h

  9-bit address, h = bit 8, l = bits 7-0
*/
byte ppu_oam_write_posl = 0x00, ppu_oam_write_posh = 0x00;

void mmio_w2102(byte value) {
  ppu_oam_write_posl = value;
  ppu.oam_write_pos = ((ppu_oam_write_posh << 8) | (ppu_oam_write_posl)) * 2;
}

void mmio_w2103(byte value) {
  ppu_oam_write_posh = value & 0x01;
  ppu.oam_write_pos = ((ppu_oam_write_posh << 8) | (ppu_oam_write_posl)) * 2;
}

byte ppu_oam_latch_data = 0;

/*
  $2104 : OAM write

  write one byte to OAM data. even writes (bit 0 = 0) are cached
  to the OAM latch, and no data is transferred to OAM ram. odd writes
  (bit 0 = 1) write the latch value, and then the requested value (2 bytes)
  to oam data. writes to OAM address 0x0200 and above (priority / x bit 8 table)
  always write, but even writes still update the latch data.
*/
void mmio_w2104(byte value) {
  if(ppu.oam_write_pos >= 0x0200) {
    if((ppu.oam_write_pos & 1) == 0) {
      ppu_oam_latch_data = value;
    }
    oam_write(ppu.oam_write_pos, value);
  } else if((ppu.oam_write_pos & 1) == 0) {
    ppu_oam_latch_data = value;
  } else {
    oam_write((ppu.oam_write_pos & 0x03fe),     ppu_oam_latch_data);
    oam_write((ppu.oam_write_pos & 0x03fe) + 1, value);
  }
  ppu.oam_write_pos++;
  ppu.oam_write_pos &= 0x03ff;
}

/*
  $2138 : OAM read

  read one byte from OAM data. if address is even (bit 0 = 0),
  latch data is updated.
*/
byte mmio_r2138(void) {
byte r;
  r = oam_read(ppu.oam_write_pos);
  if((ppu.oam_write_pos & 1) == 0) {
    ppu_oam_latch_data = r;
  }
  ppu.oam_write_pos++;
  ppu.oam_write_pos &= 0x03ff;
  return r;
}
