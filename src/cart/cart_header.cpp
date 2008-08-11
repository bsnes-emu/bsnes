#ifdef CART_CPP

void Cartridge::read_header() {
  uint8 *rom = cart.rom;
  uint index = info.header_index;
  uint8 mapper   = rom[index + MAPPER];
  uint8 rom_type = rom[index + ROM_TYPE];
  uint8 rom_size = rom[index + ROM_SIZE];
  uint8 company  = rom[index + COMPANY];
  uint8 region   = rom[index + REGION] & 0x7f;

  //detect presence of BS-X flash cartridge connector (reads extended header information)
  bool has_bsxflash = false;
  if(rom[index - 14] == 'Z') {
    if(rom[index - 11] == 'J') {
      uint8 n13 = rom[index - 13];
      if((n13 >= 'A' && n13 <= 'Z') || (n13 >= '0' && n13 <= '9')) {
        if(company == 0x33 || (rom[index - 10] == 0x00 && rom[index - 4] == 0x00)) {
          has_bsxflash = true;
        }
      }
    }
  }

  if(has_bsxflash == true) {
    info.mapper = index == 0x7fc0 ? BSCLoROM : BSCHiROM;
  } else if(index == 0x7fc0 && cart.rom_size >= 0x401000) {
    info.mapper = ExLoROM;
  } else if(index == 0x7fc0 && mapper == 0x32) {
    info.mapper = ExLoROM;
  } else if(index == 0x7fc0) {
    info.mapper = LoROM;
  } else if(index == 0xffc0) {
    info.mapper = HiROM;
  } else { //index == 0x40ffc0
    info.mapper = ExHiROM;
  }

  if(mapper == 0x20 && (rom_type == 0x13 || rom_type == 0x14 || rom_type == 0x15 || rom_type == 0x1a)) {
    info.superfx = true;
  }

  if(mapper == 0x23 && (rom_type == 0x34 || rom_type == 0x35)) {
    info.sa1 = true;
  }

  if(mapper == 0x35 && rom_type == 0x55) {
    info.srtc = true;
  }

  if(mapper == 0x32 && (rom_type == 0x43 || rom_type == 0x45)) {
    info.sdd1 = true;
  }

  if(mapper == 0x3a && (rom_type == 0xf5 || rom_type == 0xf9)) {
    info.spc7110 = true;
    info.spc7110rtc = (rom_type == 0xf9);
    info.mapper = SPC7110ROM;
  }

  if(mapper == 0x20 && rom_type == 0xf3) {
    info.cx4 = true;
  }

  if((mapper == 0x20 || mapper == 0x21) && rom_type == 0x03) {
    info.dsp1 = true;
  }

  if(mapper == 0x30 && rom_type == 0x05 && company != 0xb2) {
    info.dsp1 = true;
  }

  if(mapper == 0x31 && (rom_type == 0x03 || rom_type == 0x05)) {
    info.dsp1 = true;
  }

  if(info.dsp1 == true) {
    if((mapper & 0x2f) == 0x20 && cart.rom_size <= 0x100000) {
      info.dsp1_mapper = DSP1LoROM1MB;
    } else if((mapper & 0x2f) == 0x20) {
      info.dsp1_mapper = DSP1LoROM2MB;
    } else if((mapper & 0x2f) == 0x21) {
      info.dsp1_mapper = DSP1HiROM;
    }
  }

  if(mapper == 0x20 && rom_type == 0x05) {
    info.dsp2 = true;
  }

  if(mapper == 0x30 && rom_type == 0x05 && company == 0xb2) {
    info.dsp3 = true;
  }

  if(mapper == 0x30 && rom_type == 0x03) {
    info.dsp4 = true;
  }

  if(mapper == 0x30 && rom_type == 0x25) {
    info.obc1 = true;
  }

  if(mapper == 0x30 && rom_type == 0xf6 && rom_size >= 10) {
    info.st010 = true;
  }

  if(mapper == 0x30 && rom_type == 0xf6 && rom_size < 10) {
    info.st011 = true;
  }

  if(mapper == 0x30 && rom_type == 0xf5) {
    info.st018 = true;
  }

  if(rom[info.header_index + RAM_SIZE] & 7) {
    info.ram_size = 1024 << (rom[info.header_index + RAM_SIZE] & 7);
  } else {
    info.ram_size = 0;
  }

  //0, 1, 13 = NTSC; 2 - 12 = PAL
  info.region = (region <= 1 || region >= 13) ? NTSC : PAL;

  memcpy(&info.name, &rom[info.header_index + CART_NAME], 21);
  info.name[21] = 0;
  trim(info.name);

  //convert undisplayable characters (half-width katakana, etc) to '?' characters
  for(int i = 0; i < 21; i++) {
    if(info.name[i] & 0x80) info.name[i] = '?';
  }

  //always display something
  if(!info.name[0]) strcpy(info.name, "(untitled)");
}

void Cartridge::find_header() {
  int32 score_lo = 0, score_hi = 0, score_ex = 0;
  uint8_t *rom = cart.rom;

  if(cart.rom_size < 0x010000) {
    //cart too small to be anything but lorom
    info.header_index = 0x007fc0;
    return;
  }

  if((rom[0x7fc0 + MAPPER] & ~0x10) == 0x20) score_lo++;
  if((rom[0xffc0 + MAPPER] & ~0x10) == 0x21) score_hi++;

  if(rom[0x7fc0 + ROM_TYPE] < 0x08) score_lo++;
  if(rom[0xffc0 + ROM_TYPE] < 0x08) score_hi++;

  if(rom[0x7fc0 + ROM_SIZE] < 0x10) score_lo++;
  if(rom[0xffc0 + ROM_SIZE] < 0x10) score_hi++;

  if(rom[0x7fc0 + RAM_SIZE] < 0x08) score_lo++;
  if(rom[0xffc0 + RAM_SIZE] < 0x08) score_hi++;

  if(rom[0x7fc0 + REGION] < 14) score_lo++;
  if(rom[0xffc0 + REGION] < 14) score_hi++;

  if(rom[0x7fc0 + COMPANY] < 3) score_lo++;
  if(rom[0xffc0 + COMPANY] < 3) score_hi++;

  if(rom[0x7fc0 + RESH] & 0x80) score_lo += 2;
  if(rom[0xffc0 + RESH] & 0x80) score_hi += 2;

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

  if(cart.rom_size < 0x401000) {
    score_ex = 0;
  } else {
    if(rom[0x7fc0 + MAPPER] == 0x32) score_lo++;
    else score_ex += 12;
  }

  if(score_lo >= score_hi && score_lo >= score_ex) {
    info.header_index = 0x007fc0;
  } else if(score_hi >= score_ex) {
    info.header_index = 0x00ffc0;
  } else {
    info.header_index = 0x40ffc0;
  }
}

#endif //ifdef CART_CPP
