void Cartridge::read_header() {
  info.srtc = false;
  info.sdd1 = false;
  info.c4   = false;
  info.dsp1 = false;
  info.dsp2 = false;
  info.obc1 = false;

  info.dsp1_mapper = 0;

  if(info.header_index == 0x7fc0 && info.rom_size >= 0x401000) {
    info.mapper = EXLOROM;
    strcpy(info.pcb, "UNL-EXLOROM");
  } else if(info.header_index == 0x7fc0 && rom[info.header_index + MAPPER] == 0x32) {
    info.mapper = EXLOROM;
    strcpy(info.pcb, "UNL-EXLOROM");
  } else if(info.header_index == 0x7fc0) {
    info.mapper = LOROM;
    strcpy(info.pcb, "UNL-LOROM");
  } else if(info.header_index == 0xffc0) {
    info.mapper = HIROM;
    strcpy(info.pcb, "UNL-HIROM");
  } else { //info.header_index == 0x40ffc0
    info.mapper = EXHIROM;
    strcpy(info.pcb, "UNL-EXHIROM");
  }

uint8 mapper   = rom[info.header_index + MAPPER];
uint8 rom_type = rom[info.header_index + ROM_TYPE];
  if(mapper == 0x35 && rom_type == 0x55) {
    info.srtc = true;
  }

  if(mapper == 0x32 && (rom_type == 0x43 || rom_type == 0x45)) {
    info.sdd1 = true;
  }

  if(mapper == 0x20 && rom_type == 0xf3) {
    info.c4 = true;
  }

  if((mapper == 0x20 || mapper == 0x21) && rom_type == 0x03) {
    info.dsp1 = true;
  }

  if(mapper == 0x30 && rom_type == 0x05) {
    info.dsp1 = true;
  }

  if(mapper == 0x31 && (rom_type == 0x03 || rom_type == 0x05)) {
    info.dsp1 = true;
  }

  if(info.dsp1 == true) {
    if((mapper & 0x2f) == 0x20 && info.rom_size <= 0x100000) {
      info.dsp1_mapper = DSP1_LOROM_1MB;
    } else if((mapper & 0x2f) == 0x20) {
      info.dsp1_mapper = DSP1_LOROM_2MB;
    } else if((mapper & 0x2f) == 0x21) {
      info.dsp1_mapper = DSP1_HIROM;
    }
  }

  if(mapper == 0x20 && rom_type == 0x05) {
    info.dsp2 = true;
  }

  if(mapper == 0x30 && rom_type == 0x25) {
    info.obc1 = true;
  }

  if(mapper == 0x30 && rom_type == 0xf6) {
    info.st010 = true;
  }

  info.cart_mmio = info.c4 | info.dsp1 | info.dsp2 | info.obc1;

  if(rom[info.header_index + RAM_SIZE] & 7) {
    info.ram_size = 1024 << (rom[info.header_index + RAM_SIZE] & 7);
  } else {
    info.ram_size = 0;
  }

  memcpy(&info.name, &rom[info.header_index + CART_NAME], 21);
  info.name[21] = 0;

  for(int i = 0; i < 22; i++) {
    if(info.name[i] & 0x80) {
      info.name[i] = '?';
    }
  }
}

void Cartridge::find_header() {
int32 score_lo = 0,
      score_hi = 0,
      score_ex = 0;

  if(info.rom_size < 0x010000) {
  //cart too small to be anything but lorom
    info.header_index = 0x007fc0;
    return;
  }

  if((rom[0x7fc0 + MAPPER] & ~0x10) == 0x20)score_lo++;
  if((rom[0xffc0 + MAPPER] & ~0x10) == 0x21)score_hi++;

  if(rom[0x7fc0 + ROM_TYPE] < 0x08)score_lo++;
  if(rom[0xffc0 + ROM_TYPE] < 0x08)score_hi++;

  if(rom[0x7fc0 + ROM_SIZE] < 0x10)score_lo++;
  if(rom[0xffc0 + ROM_SIZE] < 0x10)score_hi++;

  if(rom[0x7fc0 + RAM_SIZE] < 0x08)score_lo++;
  if(rom[0xffc0 + RAM_SIZE] < 0x08)score_hi++;

  if(rom[0x7fc0 + REGION] < 14)score_lo++;
  if(rom[0xffc0 + REGION] < 14)score_hi++;

  if(rom[0x7fc0 + LICENSE] < 3)score_lo++;
  if(rom[0xffc0 + LICENSE] < 3)score_hi++;

  if(rom[0x7fc0 + RESH] & 0x80)score_lo += 2;
  if(rom[0xffc0 + RESH] & 0x80)score_hi += 2;

uint16 cksum, icksum;
  cksum  = rom[0x7fc0 +  CKSUM] | (rom[0x7fc0 +  CKSUM + 1] << 8);
  icksum = rom[0x7fc0 + ICKSUM] | (rom[0x7fc0 + ICKSUM + 1] << 8);
  if((cksum + icksum) == 0xffff && (cksum != 0) && (icksum != 0)) {
    score_lo += 8;
  }

  cksum  = rom[0xffc0 +  CKSUM] | (rom[0xffc0 +  CKSUM + 1] << 8);
  icksum = rom[0xffc0 + ICKSUM] | (rom[0xffc0 + ICKSUM + 1] << 8);
  if((cksum + icksum) == 0xffff && (cksum != 0) && (icksum != 0)) {
    score_hi += 8;
  }

  if(info.rom_size < 0x401000) {
    score_ex = 0;
  } else {
    if(rom[0x7fc0 + MAPPER] == 0x32)score_lo++;
    else score_ex += 16;
  }

  if(score_lo >= score_hi && score_lo >= score_ex) {
    info.header_index = 0x007fc0;
  } else if(score_hi >= score_ex) {
    info.header_index = 0x00ffc0;
  } else {
    info.header_index = 0x40ffc0;
  }
}
