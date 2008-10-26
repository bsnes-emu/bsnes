#ifdef CART_CPP

void Cartridge::read_header(cartinfo_t &info, const uint8_t *data, unsigned size) {
  info.reset();
  unsigned index = find_header(data, size);

  //detect BS-X flash carts
  if(data[index + 0x13] == 0x00 || data[index + 0x13] == 0xff) {
    if(data[index + 0x14] == 0x00) {
      const uint8_t n15 = data[index + 0x15];
      if(n15 == 0x00 || n15 == 0x80 || n15 == 0x84 || n15 == 0x9c || n15 == 0xbc || n15 == 0xfc) {
        if(data[index + 0x1a] == 0x33 || data[index + 0x1a] == 0xff) {
          info.type = TypeBSX;
          info.mapper = BSXROM;
          info.region = NTSC;  //BS-X only released in Japan
          return;
        }
      }
    }
  }

  //detect Sufami Turbo carts
  if(!memcmp(data, "BANDAI SFC-ADX", 14)) {
    if(!memcmp(data + 16, "SFC-ADX BACKUP", 14)) {
      info.type = TypeSufamiTurboBIOS;
    } else {
      info.type = TypeSufamiTurbo;
    }
    info.mapper = STROM;
    info.region = NTSC;  //Sufami Turbo only released in Japan
    return;
  }

  //standard cart
  uint8 mapper   = data[index + MAPPER];
  uint8 rom_type = data[index + ROM_TYPE];
  uint8 rom_size = data[index + ROM_SIZE];
  uint8 company  = data[index + COMPANY];
  uint8 region   = data[index + REGION] & 0x7f;

  //detect presence of BS-X flash cartridge connector (reads extended header information)
  if(data[index - 14] == 'Z') {
    if(data[index - 11] == 'J') {
      uint8 n13 = data[index - 13];
      if((n13 >= 'A' && n13 <= 'Z') || (n13 >= '0' && n13 <= '9')) {
        if(company == 0x33 || (data[index - 10] == 0x00 && data[index - 4] == 0x00)) {
          info.bsxslot = true;
        }
      }
    }
  }

  if(info.bsxslot == true) {
    if(!memcmp(data + index, "Satellaview BS-X     ", 21)) {
      //BS-X base cart
      info.type = TypeBSXBIOS;
      info.mapper = BSXROM;
    } else {
      info.type = TypeBSC;
      info.mapper = (index == 0x7fc0 ? BSCLoROM : BSCHiROM);
    }
    return;
  }

  info.type = TypeNormal;

  if(index == 0x7fc0 && size >= 0x401000) {
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
    if((mapper & 0x2f) == 0x20 && size <= 0x100000) {
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

  if(data[index + RAM_SIZE] & 7) {
    info.ram_size = 1024 << (data[index + RAM_SIZE] & 7);
  } else {
    info.ram_size = 0;
  }

  //0, 1, 13 = NTSC; 2 - 12 = PAL
  info.region = (region <= 1 || region >= 13) ? NTSC : PAL;
}

unsigned Cartridge::find_header(const uint8_t *data, unsigned size) {
  unsigned score_lo = score_header(data, size, 0x007fc0);
  unsigned score_hi = score_header(data, size, 0x00ffc0);
  unsigned score_ex = score_header(data, size, 0x40ffc0);
  if(score_ex) score_ex += 4; //favor ExHiROM on images > 32mbits

  if(score_lo >= score_hi && score_lo >= score_ex) {
    return 0x007fc0;
  } else if(score_hi >= score_ex) {
    return 0x00ffc0;
  } else {
    return 0x40ffc0;
  }
}

unsigned Cartridge::score_header(const uint8_t *data, unsigned size, unsigned addr) {
  if(size < addr + 64) return 0; //image too small to contain header at this location?
  int score = 0;

  uint16 resetvector = data[addr + RESETV] | (data[addr + RESETV + 1] << 8);
  uint16 checksum    = data[addr +  CKSUM] | (data[addr +  CKSUM + 1] << 8);
  uint16 ichecksum   = data[addr + ICKSUM] | (data[addr + ICKSUM + 1] << 8);

  uint8 resetop = data[(addr & ~0x7fff) | (resetvector & 0x7fff)]; //first opcode executed upon reset
  uint8 mapper  = data[addr + MAPPER] & ~0x10; //mask off irrelevent FastROM-capable bit

  //$00:[000-7fff] contains uninitialized RAM and MMIO.
  //reset vector must point to ROM at $00:[8000-ffff] to be considered valid.
  if(resetvector < 0x8000) return 0;

  //some images duplicate the header in multiple locations, and others have completely
  //invalid header information that cannot be relied upon.
  //below code will analyze the first opcode executed at the specified reset vector to
  //determine the probability that this is the correct header.

  //most likely opcodes
  if(resetop == 0x78 //sei
  || resetop == 0x18 //clc (clc; xce)
  || resetop == 0x38 //sec (sec; xce)
  || resetop == 0x9c //stz $nnnn (stz $4200)
  || resetop == 0x4c //jmp $nnnn
  || resetop == 0x5c //jml $nnnnnn
  ) score += 8;

  //plausible opcodes
  if(resetop == 0xc2 //rep #$nn
  || resetop == 0xe2 //sep #$nn
  || resetop == 0xad //lda $nnnn
  || resetop == 0xae //ldx $nnnn
  || resetop == 0xac //ldy $nnnn
  || resetop == 0xaf //lda $nnnnnn
  || resetop == 0xa9 //lda #$nn
  || resetop == 0xa2 //ldx #$nn
  || resetop == 0xa0 //ldy #$nn
  || resetop == 0x20 //jsr $nnnn
  || resetop == 0x22 //jsl $nnnnnn
  ) score += 4;

  //implausible opcodes
  if(resetop == 0x40 //rti
  || resetop == 0x60 //rts
  || resetop == 0x6b //rtl
  || resetop == 0xcd //cmp $nnnn
  || resetop == 0xec //cpx $nnnn
  || resetop == 0xcc //cpy $nnnn
  ) score -= 4;

  //least likely opcodes
  if(resetop == 0x00 //brk #$nn
  || resetop == 0x02 //cop #$nn
  || resetop == 0xdb //stp
  || resetop == 0x42 //wdm
  || resetop == 0xff //sbc $nnnnnn,x
  ) score -= 8;

  //at times, both the header and reset vector's first opcode will match ...
  //fallback and rely on info validity in these cases to determine more likely header.

  //a valid checksum is the biggest indicator of a valid header.
  if((checksum + ichecksum) == 0xffff && (checksum != 0) && (ichecksum != 0)) score += 4;

  if(addr == 0x007fc0 && mapper == 0x20) score += 2; //0x20 is usually LoROM
  if(addr == 0x00ffc0 && mapper == 0x21) score += 2; //0x21 is usually HiROM
  if(addr == 0x007fc0 && mapper == 0x22) score += 2; //0x22 is usually ExLoROM
  if(addr == 0x40ffc0 && mapper == 0x25) score += 2; //0x25 is usually ExHiROM

  if(data[addr + COMPANY] == 0x33) score += 2; //0x33 indicates extended header
  if(data[addr + ROM_TYPE] < 0x08) score++;
  if(data[addr + ROM_SIZE] < 0x10) score++;
  if(data[addr + RAM_SIZE] < 0x08) score++;
  if(data[addr + REGION] < 14) score++;

  if(score < 0) score = 0;
  return score;
}

#endif //ifdef CART_CPP
