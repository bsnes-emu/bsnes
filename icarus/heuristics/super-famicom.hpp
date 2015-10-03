struct SuperFamicomCartridge {
  SuperFamicomCartridge(const uint8_t *data, unsigned size);

  string markup;

//private:
  auto readHeader(const uint8_t *data, unsigned size) -> void;
  auto findHeader(const uint8_t *data, unsigned size) -> unsigned;
  auto scoreHeader(const uint8_t *data, unsigned size, unsigned addr) -> unsigned;

  enum HeaderField : unsigned {
    CartName    = 0x00,
    Mapper      = 0x15,
    RomType     = 0x16,
    RomSize     = 0x17,
    RamSize     = 0x18,
    CartRegion  = 0x19,
    Company     = 0x1a,
    Version     = 0x1b,
    Complement  = 0x1c,  //inverse checksum
    Checksum    = 0x1e,
    ResetVector = 0x3c,
  };

  enum Mode : unsigned {
    ModeNormal,
    ModeBsxSlotted,
    ModeBsx,
    ModeSufamiTurbo,
    ModeSuperGameBoy,
  };

  enum Type : unsigned {
    TypeNormal,
    TypeBsxSlotted,
    TypeBsxBios,
    TypeBsx,
    TypeSufamiTurboBios,
    TypeSufamiTurbo,
    TypeSuperGameBoy1Bios,
    TypeSuperGameBoy2Bios,
    TypeGameBoy,
    TypeUnknown,
  };

  enum Region : unsigned {
    NTSC,
    PAL,
  };

  enum MemoryMapper : unsigned {
    LoROM,
    HiROM,
    ExLoROM,
    ExHiROM,
    SuperFXROM,
    SA1ROM,
    SPC7110ROM,
    BSCLoROM,
    BSCHiROM,
    BSXROM,
    STROM,
  };

  enum DSP1MemoryMapper : unsigned {
    DSP1Unmapped,
    DSP1LoROM1MB,
    DSP1LoROM2MB,
    DSP1HiROM,
  };

  bool loaded;            //is a base cartridge inserted?
  unsigned crc32;         //crc32 of all cartridges (base+slot(s))
  unsigned rom_size;
  unsigned ram_size;
  bool firmware_required;  //true if firmware is required for emulation
  bool firmware_appended;  //true if firmware is present at end of data

  Mode mode;
  Type type;
  Region region;
  MemoryMapper mapper;
  DSP1MemoryMapper dsp1_mapper;

  bool has_bsx_slot;
  bool has_superfx;
  bool has_sa1;
  bool has_sharprtc;
  bool has_epsonrtc;
  bool has_sdd1;
  bool has_spc7110;
  bool has_cx4;
  bool has_dsp1;
  bool has_dsp2;
  bool has_dsp3;
  bool has_dsp4;
  bool has_obc1;
  bool has_st010;
  bool has_st011;
  bool has_st018;
};

SuperFamicomCartridge::SuperFamicomCartridge(const uint8_t *data, unsigned size) {
  firmware_required = false;
  firmware_appended = false;

  //skip copier header
  if((size & 0x7fff) == 512) data += 512, size -= 512;

  if(size < 0x8000) return;

  readHeader(data, size);

  if(type == TypeGameBoy) return;
  if(type == TypeBsx) return;
  if(type == TypeSufamiTurbo) return;

  const char* range = (rom_size > 0x200000) || (ram_size > 32 * 1024) ? "0000-7fff" : "0000-ffff";
  markup.append("cartridge region=", region == NTSC ? "NTSC" : "PAL", "\n");

  //detect appended firmware

  if(has_dsp1) {
    firmware_required = true;
    if((size & 0x7fff) == 0x2000) {
      firmware_appended = true;
      rom_size -= 0x2000;
    }
  }

  if(has_dsp2) {
    firmware_required = true;
    if((size & 0x7fff) == 0x2000) {
      firmware_appended = true;
      rom_size -= 0x2000;
    }
  }

  if(has_dsp3) {
    firmware_required = true;
    if((size & 0x7fff) == 0x2000) {
      firmware_appended = true;
      rom_size -= 0x2000;
    }
  }

  if(has_dsp4) {
    firmware_required = true;
    if((size & 0x7fff) == 0x2000) {
      firmware_appended = true;
      rom_size -= 0x2000;
    }
  }

  if(has_st010) {
    firmware_required = true;
    if((size & 0xffff) == 0xd000) {
      firmware_appended = true;
      rom_size -= 0xd000;
    }
  }

  if(has_st011) {
    firmware_required = true;
    if((size & 0xffff) == 0xd000) {
      firmware_appended = true;
      rom_size -= 0xd000;
    }
  }

  if(has_st018) {
    firmware_required = true;
    if((size & 0x3ffff) == 0x28000) {
      firmware_appended = true;
      rom_size -= 0x28000;
    }
  }

  if(has_cx4) {
    firmware_required = true;
    if((rom_size & 0x7fff) == 0xc00) {
      firmware_appended = true;
      rom_size -= 0xc00;
    }
  }

  if(type == TypeSuperGameBoy1Bios || type == TypeSuperGameBoy2Bios) {
    firmware_required = true;
    if((rom_size & 0x7fff) == 0x100) {
      firmware_appended = true;
      rom_size -= 0x100;
    }
  }

  //end firmware detection

  if(type == TypeSuperGameBoy1Bios || type == TypeSuperGameBoy2Bios) {
    markup.append(
      "  rom name=program.rom size=0x", hex(rom_size), "\n"
      "  map id=rom address=00-7f,80-ff:8000-ffff mask=0x8000\n"
      "  icd2 revision=1\n"
      "    rom name=sgb.boot.rom size=0x100\n"
      "    map id=io address=00-3f,80-bf:6000-7fff\n"
    );
  }

  else if(has_cx4) {
    markup.append(
      "  hitachidsp model=HG51B169 frequency=20000000\n"
      "    rom id=program name=program.rom size=0x", hex(rom_size), "\n"
      "    rom id=data name=cx4.data.rom size=0xc00\n"
      "    ram id=data size=0xc00\n"
      "    map id=io address=00-3f,80-bf:6000-7fff\n"
      "    map id=rom address=00-7f,80-ff:8000-ffff mask=0x8000\n"
      "    map id=ram address=70-77:0000-7fff\n"
    );
  }

  else if(has_spc7110) {
    markup.append(
      "  spc7110\n"
      "    rom id=program name=program.rom size=0x100000\n"
      "    rom id=data name=data.rom size=0x", hex(rom_size - 0x100000), "\n"
      "    ram name=save.ram size=0x", hex(ram_size), "\n"
      "    map id=io address=00-3f,80-bf:4800-483f\n"
      "    map id=io address=50:0000-ffff\n"
      "    map id=rom address=00-3f,80-bf:8000-ffff\n"
      "    map id=rom address=c0-ff:0000-ffff\n"
      "    map id=ram address=00-3f,80-bf:6000-7fff mask=0xe000\n"
    );
  }

  else if(has_sdd1) {
    markup.append(
      "  sdd1\n"
      "    rom name=program.rom size=0x", hex(rom_size), "\n"
    );
    if(ram_size > 0) markup.append(
      "    ram name=save.ram size=0x", hex(ram_size), "\n"
    );
    markup.append(
      "    map id=io address=00-3f,80-bf:4800-4807\n"
      "    map id=rom address=00-3f,80-bf:8000-ffff mask=0x8000\n"
      "    map id=rom address=c0-ff:0000-ffff\n"
    );
    if(ram_size > 0) markup.append(
      "    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000\n"
      "    map id=ram address=70-7f:0000-7fff\n"
    );
  }

  else if(mapper == LoROM) {
    markup.append(
      "  rom name=program.rom size=0x", hex(rom_size), "\n"
    );
    if(ram_size > 0) markup.append(
      "  ram name=save.ram size=0x", hex(ram_size), "\n"
    );
    markup.append(
      "  map id=rom address=00-7f,80-ff:8000-ffff mask=0x8000\n"
    );
    if(ram_size > 0) markup.append(
      "  map id=ram address=70-7f,f0-ff:", range, "\n"
    );
  }

  else if(mapper == HiROM) {
    markup.append(
      "  rom name=program.rom size=0x", hex(rom_size), "\n"
    );
    if(ram_size > 0) markup.append(
      "  ram name=save.ram size=0x", hex(ram_size), "\n"
    );
    markup.append(
      "  map id=rom address=00-3f,80-bf:8000-ffff\n"
      "  map id=rom address=40-7f,c0-ff:0000-ffff\n"
    );
    if(ram_size > 0) markup.append(
      "  map id=ram address=10-3f,90-bf:6000-7fff mask=0xe000\n"
    );
  }

  else if(mapper == ExLoROM) {
    markup.append(
      "  rom name=program.rom size=0x", hex(rom_size), "\n"
    );
    if(ram_size > 0) markup.append(
      "  ram name=save.ram size=0x", hex(ram_size), "\n"
    );
    markup.append(
      "  map id=rom address=00-3f,80-bf:8000-ffff mask=0x8000\n"
      "  map id=rom address=40-7f:0000-ffff\n"
    );
    if(ram_size > 0) markup.append(
      "  map id=ram address=20-3f,a0-bf:6000-7fff\n"
      "  map id=ram address=70-7f:0000-7fff\n"
    );
  }

  else if(mapper == ExHiROM) {
    markup.append(
      "  rom name=program.rom size=0x", hex(rom_size), "\n"
    );
    if(ram_size > 0) markup.append(
      "  ram name=save.ram size=0x", hex(ram_size), "\n"
    );
    markup.append(
      "  map id=rom address=00-3f:8000-ffff base=0x400000\n"
      "  map id=rom address=40-7f:0000-ffff base=0x400000\n"
      "  map id=rom address=80-bf:8000-ffff mask=0xc00000\n"
      "  map id=rom address=c0-ff:0000-ffff mask=0xc00000\n"
    );
    if(ram_size > 0) markup.append(
      "  map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000\n"
      "  map id=ram address=70-7f:", range, "\n"
    );
  }

  else if(mapper == SuperFXROM) {
    markup.append(
      "  superfx revision=3\n"
      "    rom name=program.rom size=0x", hex(rom_size), "\n"
    );
    if(ram_size > 0) markup.append(
      "    ram name=save.ram size=0x", hex(ram_size), "\n"
    );
    markup.append(
      "    map id=io address=00-3f,80-bf:3000-32ff\n"
      "    map id=rom address=00-3f,80-bf:8000-ffff mask=0x8000\n"
      "    map id=rom address=40-5f,c0-df:0000-ffff\n"
    );
    if(ram_size > 0) markup.append(
      "    map id=ram address=00-3f,80-bf:6000-7fff size=0x2000\n"
      "    map id=ram address=70-71,f0-f1:0000-ffff\n"
    );
  }

  else if(mapper == SA1ROM) {
    markup.append(
      "  sa1\n"
      "    rom name=program.rom size=0x", hex(rom_size), "\n"
    );
    if(ram_size > 0) markup.append(
      "    ram id=bitmap name=save.ram size=0x", hex(ram_size), "\n"
    );
    markup.append(
      "    ram id=internal size=0x800\n"
      "    map id=io address=00-3f,80-bf:2200-23ff\n"
      "    map id=rom address=00-3f,80-bf:8000-ffff\n"
      "    map id=rom address=c0-ff:0000-ffff\n"
    );
    if(ram_size > 0) markup.append(
      "    map id=bwram address=00-3f,80-bf:6000-7fff\n"
      "    map id=bwram address=40-4f:0000-ffff\n"
    );
    markup.append(
      "    map id=iram address=00-3f,80-bf:3000-37ff\n"
    );
  }

  else if(mapper == BSCLoROM) {
    markup.append(
      "  rom name=program.rom size=0x", hex(rom_size), "\n"
      "  ram name=save.ram size=0x", hex(ram_size), "\n"
      "  map id=rom address=00-1f:8000-ffff base=0x000000 mask=0x8000\n"
      "  map id=rom address=20-3f:8000-ffff base=0x100000 mask=0x8000\n"
      "  map id=rom address=80-9f:8000-ffff base=0x200000 mask=0x8000\n"
      "  map id=rom address=a0-bf:8000-ffff base=0x100000 mask=0x8000\n"
      "  map id=ram address=70-7f,f0-ff:0000-7fff\n"
      "  satellaview\n"
      "    map id=rom address=c0-ef:0000-ffff\n"
    );
  }

  else if(mapper == BSCHiROM) {
    markup.append(
      "  rom name=program.rom size=0x", hex(rom_size), "\n"
      "  ram name=save.ram size=0x", hex(ram_size), "\n"
      "  map id=rom address=00-1f,80-9f:8000-ffff\n"
      "  map id=rom address=40-5f,c0-df:0000-ffff\n"
      "  map id=ram address=20-3f,a0-bf:6000-7fff\n"
      "  satellaview\n"
      "    map id=rom address=20-3f,a0-bf:8000-ffff\n"
      "    map id=rom address=60-7f,e0-ff:0000-ffff\n"
    );
  }

  else if(mapper == BSXROM) {
    markup.append(
      "  mcc\n"
      "    rom name=program.rom size=0x", hex(rom_size), "\n"
      "    ram id=save name=save.ram size=0x", hex(ram_size), "\n"
      "    ram id=download name=download.ram size=0x80000\n"
      "    map id=io address=00-3f,80-bf:5000-5fff\n"
      "    map id=rom address=00-3f,80-bf:8000-ffff\n"
      "    map id=rom address=40-7f,c0-ff:0000-ffff\n"
      "    map id=ram address=20-3f:6000-7fff\n"
    );
  }

  else if(mapper == STROM) {
    markup.append(
      "  rom name=program.rom size=0x", hex(rom_size), "\n"
      "  map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000\n"
      "  sufamiturbo\n"
      "    map id=rom address=20-3f,a0-bf:8000-ffff mask=0x8000\n"
      "    map id=ram address=60-6f,e0-ef:0000-ffff\n"
      "  sufamiturbo\n"
      "    map id=rom address=40-5f,c0-df:0000-7fff mask=0x8000\n"
      "    map id=rom address=40-5f,c0-df:8000-ffff mask=0x8000\n"
      "    map id=ram address=70-7f,f0-ff:0000-ffff\n"
    );
  }

  if(has_sharprtc) {
    markup.append(
      "  sharprtc\n"
      "    ram name=rtc.ram size=0x10\n"
      "    map id=io address=00-3f,80-bf:2800-2801\n"
    );
  }

  if(has_epsonrtc) {
    markup.append(
      "  epsonrtc\n"
      "    ram name=rtc.ram size=0x10\n"
      "    map id=io address=00-3f,80-bf:4840-4842\n"
    );
  }

  if(has_obc1) {
    markup.append(
      "  obc1\n"
      "    ram name=save.ram size=0x2000\n"
      "    map id=io address=00-3f,80-bf:6000-7fff\n"
    );
  }

  if(has_dsp1) {
    markup.append(
      "  necdsp model=uPD7725 frequency=8000000\n"
      "    rom id=program name=dsp1b.program.rom size=0x1800\n"
      "    rom id=data name=dsp1b.data.rom size=0x800\n"
      "    ram id=data size=0x200\n"
    );
    if(dsp1_mapper == DSP1LoROM1MB) markup.append(
      "    map id=io address=20-3f,a0-bf:8000-ffff select=0x4000\n"
    );
    if(dsp1_mapper == DSP1LoROM2MB) markup.append(
      "    map id=io address=60-6f,e0-ef:0000-7fff select=0x4000\n"
    );
    if(dsp1_mapper == DSP1HiROM) markup.append(
      "    map id=io address=00-1f,80-9f:6000-7fff select=0x1000\n"
    );
  }

  if(has_dsp2) {
    markup.append(
      "  necdsp model=uPD7725 frequency=8000000\n"
      "    rom id=program name=dsp2.program.rom size=0x1800\n"
      "    rom id=data name=dsp2.data.rom size=0x800\n"
      "    ram id=data size=0x200\n"
      "    map id=io address=20-3f,a0-bf:8000-ffff select=0x4000\n"
    );
  }

  if(has_dsp3) {
    markup.append(
      "  necdsp model=uPD7725 frequency=8000000\n"
      "    rom id=program name=dsp3.program.rom size=0x1800\n"
      "    rom id=data name=dsp3.data.rom size=0x800\n"
      "    ram id=data size=0x200\n"
      "    map id=io address=20-3f,a0-bf:8000-ffff select=0x4000\n"
    );
  }

  if(has_dsp4) {
    markup.append(
      "  necdsp model=uPD7725 frequency=8000000\n"
      "    rom id=program name=dsp4.program.rom size=0x1800\n"
      "    rom id=data name=dsp4.data.rom size=0x800\n"
      "    ram id=data size=0x200\n"
      "    map id=io address=30-3f,b0-bf:8000-ffff select=0x4000\n"
    );
  }

  if(has_st010) {
    markup.append(
      "  necdsp model=uPD96050 frequency=11000000\n"
      "    rom id=program name=st010.program.rom size=0xc000\n"
      "    rom id=data name=st010.data.rom size=0x1000\n"
      "    ram id=data name=save.ram size=0x1000\n"
      "    map id=io address=60-67,e0-e7:0000-3fff select=0x0001\n"
      "    map id=ram address=68-6f,e8-ef:0000-7fff\n"
    );
  }

  if(has_st011) {
    markup.append(
      "  necdsp model=uPD96050 frequency=15000000\n"
      "    rom id=program name=st011.program.rom size=0xc000\n"
      "    rom id=data name=st011.data.rom size=0x1000\n"
      "    ram id=data name=save.ram size=0x1000\n"
      "    map id=io address=60-67,e0-e7:0000-3fff select=0x0001\n"
      "    map id=ram address=68-6f,e8-ef:0000-7fff\n"
    );
  }

  if(has_st018) {
    markup.append(
      "  armdsp frequency=21477272\n"
      "    rom id=program name=st018.program.rom size=0x20000\n"
      "    rom id=data name=st018.data.rom size=0x8000\n"
      "    ram name=save.ram size=0x4000\n"
      "    map id=io address=00-3f,80-bf:3800-38ff\n"
    );
  }
}

auto SuperFamicomCartridge::readHeader(const uint8_t *data, unsigned size) -> void {
  type        = TypeUnknown;
  mapper      = LoROM;
  dsp1_mapper = DSP1Unmapped;
  region      = NTSC;
  rom_size    = size;
  ram_size    = 0;

  has_bsx_slot   = false;
  has_superfx    = false;
  has_sa1        = false;
  has_sharprtc   = false;
  has_epsonrtc   = false;
  has_sdd1       = false;
  has_spc7110    = false;
  has_cx4        = false;
  has_dsp1       = false;
  has_dsp2       = false;
  has_dsp3       = false;
  has_dsp4       = false;
  has_obc1       = false;
  has_st010      = false;
  has_st011      = false;
  has_st018      = false;

  //=====================
  //detect Game Boy carts
  //=====================

  if(size >= 0x0140) {
    if(data[0x0104] == 0xce && data[0x0105] == 0xed && data[0x0106] == 0x66 && data[0x0107] == 0x66
    && data[0x0108] == 0xcc && data[0x0109] == 0x0d && data[0x010a] == 0x00 && data[0x010b] == 0x0b) {
      type = TypeGameBoy;
      return;
    }
  }

  if(size < 32768) {
    type = TypeUnknown;
    return;
  }

  const unsigned index = findHeader(data, size);
  const uint8_t mapperid = data[index + Mapper];
  const uint8_t rom_type = data[index + RomType];
  const uint8_t rom_size = data[index + RomSize];
  const uint8_t company  = data[index + Company];
  const uint8_t regionid = data[index + CartRegion] & 0x7f;

  ram_size = 1024 << (data[index + RamSize] & 7);
  if(ram_size == 1024) ram_size = 0;  //no RAM present
  if(rom_size == 0 && ram_size) ram_size = 0;  //fix for Bazooka Blitzkrieg's malformed header (swapped ROM and RAM sizes)

  //0, 1, 13 = NTSC; 2 - 12 = PAL
  region = (regionid <= 1 || regionid >= 13) ? NTSC : PAL;

  //=======================
  //detect BS-X flash carts
  //=======================

  if(data[index + 0x13] == 0x00 || data[index + 0x13] == 0xff) {
    if(data[index + 0x14] == 0x00) {
      const uint8_t n15 = data[index + 0x15];
      if(n15 == 0x00 || n15 == 0x80 || n15 == 0x84 || n15 == 0x9c || n15 == 0xbc || n15 == 0xfc) {
        if(data[index + 0x1a] == 0x33 || data[index + 0x1a] == 0xff) {
          type = TypeBsx;
          mapper = BSXROM;
          region = NTSC;  //BS-X only released in Japan
          return;
        }
      }
    }
  }

  //=========================
  //detect Sufami Turbo carts
  //=========================

  if(!memcmp(data, "BANDAI SFC-ADX", 14)) {
    if(!memcmp(data + 16, "SFC-ADX BACKUP", 14)) {
      type = TypeSufamiTurboBios;
    } else {
      type = TypeSufamiTurbo;
    }
    mapper = STROM;
    region = NTSC;  //Sufami Turbo only released in Japan
    return;         //RAM size handled outside this routine
  }

  //==========================
  //detect Super Game Boy BIOS
  //==========================

  if(!memcmp(data + index, "Super GAMEBOY2", 14)) {
    type = TypeSuperGameBoy2Bios;
    return;
  }

  if(!memcmp(data + index, "Super GAMEBOY", 13)) {
    type = TypeSuperGameBoy1Bios;
    return;
  }

  //=====================
  //detect standard carts
  //=====================

  //detect presence of BS-X flash cartridge connector (reads extended header information)
  if(data[index - 14] == 'Z') {
    if(data[index - 11] == 'J') {
      uint8_t n13 = data[index - 13];
      if((n13 >= 'A' && n13 <= 'Z') || (n13 >= '0' && n13 <= '9')) {
        if(company == 0x33 || (data[index - 10] == 0x00 && data[index - 4] == 0x00)) {
          has_bsx_slot = true;
        }
      }
    }
  }

  if(has_bsx_slot) {
    if(!memcmp(data + index, "Satellaview BS-X     ", 21)) {
      //BS-X base cart
      type = TypeBsxBios;
      mapper = BSXROM;
      region = NTSC;  //BS-X only released in Japan
      return;         //RAM size handled internally by load_cart_bsx() -> BSXCart class
    } else {
      type = TypeBsxSlotted;
      mapper = (index == 0x7fc0 ? BSCLoROM : BSCHiROM);
      region = NTSC;  //BS-X slotted cartridges only released in Japan
    }
  } else {
    //standard cart
    type = TypeNormal;

    if(index == 0x7fc0 && size >= 0x401000) {
      mapper = ExLoROM;
    } else if(index == 0x7fc0 && mapperid == 0x32) {
      mapper = ExLoROM;
    } else if(index == 0x7fc0) {
      mapper = LoROM;
    } else if(index == 0xffc0) {
      mapper = HiROM;
    } else {  //index == 0x40ffc0
      mapper = ExHiROM;
    }
  }

  if(mapperid == 0x20 && (rom_type == 0x13 || rom_type == 0x14 || rom_type == 0x15 || rom_type == 0x1a)) {
    has_superfx = true;
    mapper = SuperFXROM;
    ram_size = 1024 << (data[index - 3] & 7);
    if(ram_size == 1024) ram_size = 0;
  }

  if(mapperid == 0x23 && (rom_type == 0x32 || rom_type == 0x34 || rom_type == 0x35)) {
    has_sa1 = true;
    mapper = SA1ROM;
  }

  if(mapperid == 0x35 && rom_type == 0x55) {
    has_sharprtc = true;
  }

  if(mapperid == 0x32 && (rom_type == 0x43 || rom_type == 0x45)) {
    has_sdd1 = true;
  }

  if(mapperid == 0x3a && (rom_type == 0xf5 || rom_type == 0xf9)) {
    has_spc7110 = true;
    has_epsonrtc = (rom_type == 0xf9);
    mapper = SPC7110ROM;
  }

  if(mapperid == 0x20 && rom_type == 0xf3) {
    has_cx4 = true;
  }

  if((mapperid == 0x20 || mapperid == 0x21) && rom_type == 0x03) {
    has_dsp1 = true;
  }

  if(mapperid == 0x30 && rom_type == 0x05 && company != 0xb2) {
    has_dsp1 = true;
  }

  if(mapperid == 0x31 && (rom_type == 0x03 || rom_type == 0x05)) {
    has_dsp1 = true;
  }

  if(has_dsp1 == true) {
    if((mapperid & 0x2f) == 0x20 && size <= 0x100000) {
      dsp1_mapper = DSP1LoROM1MB;
    } else if((mapperid & 0x2f) == 0x20) {
      dsp1_mapper = DSP1LoROM2MB;
    } else if((mapperid & 0x2f) == 0x21) {
      dsp1_mapper = DSP1HiROM;
    }
  }

  if(mapperid == 0x20 && rom_type == 0x05) {
    has_dsp2 = true;
  }

  if(mapperid == 0x30 && rom_type == 0x05 && company == 0xb2) {
    has_dsp3 = true;
  }

  if(mapperid == 0x30 && rom_type == 0x03) {
    has_dsp4 = true;
  }

  if(mapperid == 0x30 && rom_type == 0x25) {
    has_obc1 = true;
  }

  if(mapperid == 0x30 && rom_type == 0xf6 && rom_size >= 10) {
    has_st010 = true;
  }

  if(mapperid == 0x30 && rom_type == 0xf6 && rom_size < 10) {
    has_st011 = true;
  }

  if(mapperid == 0x30 && rom_type == 0xf5) {
    has_st018 = true;
  }
}

auto SuperFamicomCartridge::findHeader(const uint8_t *data, unsigned size) -> unsigned {
  unsigned score_lo = scoreHeader(data, size, 0x007fc0);
  unsigned score_hi = scoreHeader(data, size, 0x00ffc0);
  unsigned score_ex = scoreHeader(data, size, 0x40ffc0);
  if(score_ex) score_ex += 4;  //favor ExHiROM on images > 32mbits

  if(score_lo >= score_hi && score_lo >= score_ex) {
    return 0x007fc0;
  } else if(score_hi >= score_ex) {
    return 0x00ffc0;
  } else {
    return 0x40ffc0;
  }
}

auto SuperFamicomCartridge::scoreHeader(const uint8_t *data, unsigned size, unsigned addr) -> unsigned {
  if(size < addr + 64) return 0;  //image too small to contain header at this location?
  int score = 0;

  uint16_t resetvector = data[addr + ResetVector] | (data[addr + ResetVector + 1] << 8);
  uint16_t checksum    = data[addr + Checksum   ] | (data[addr + Checksum    + 1] << 8);
  uint16_t complement  = data[addr + Complement ] | (data[addr + Complement  + 1] << 8);

  uint8_t resetop = data[(addr & ~0x7fff) | (resetvector & 0x7fff)];  //first opcode executed upon reset
  uint8_t mapper  = data[addr + Mapper] & ~0x10;                      //mask off irrelevent FastROM-capable bit

  //$00:[000-7fff] contains uninitialized RAM and MMIO.
  //reset vector must point to ROM at $00:[8000-ffff] to be considered valid.
  if(resetvector < 0x8000) return 0;

  //some images duplicate the header in multiple locations, and others have completely
  //invalid header information that cannot be relied upon.
  //below code will analyze the first opcode executed at the specified reset vector to
  //determine the probability that this is the correct header.

  //most likely opcodes
  if(resetop == 0x78  //sei
  || resetop == 0x18  //clc (clc; xce)
  || resetop == 0x38  //sec (sec; xce)
  || resetop == 0x9c  //stz $nnnn (stz $4200)
  || resetop == 0x4c  //jmp $nnnn
  || resetop == 0x5c  //jml $nnnnnn
  ) score += 8;

  //plausible opcodes
  if(resetop == 0xc2  //rep #$nn
  || resetop == 0xe2  //sep #$nn
  || resetop == 0xad  //lda $nnnn
  || resetop == 0xae  //ldx $nnnn
  || resetop == 0xac  //ldy $nnnn
  || resetop == 0xaf  //lda $nnnnnn
  || resetop == 0xa9  //lda #$nn
  || resetop == 0xa2  //ldx #$nn
  || resetop == 0xa0  //ldy #$nn
  || resetop == 0x20  //jsr $nnnn
  || resetop == 0x22  //jsl $nnnnnn
  ) score += 4;

  //implausible opcodes
  if(resetop == 0x40  //rti
  || resetop == 0x60  //rts
  || resetop == 0x6b  //rtl
  || resetop == 0xcd  //cmp $nnnn
  || resetop == 0xec  //cpx $nnnn
  || resetop == 0xcc  //cpy $nnnn
  ) score -= 4;

  //least likely opcodes
  if(resetop == 0x00  //brk #$nn
  || resetop == 0x02  //cop #$nn
  || resetop == 0xdb  //stp
  || resetop == 0x42  //wdm
  || resetop == 0xff  //sbc $nnnnnn,x
  ) score -= 8;

  //at times, both the header and reset vector's first opcode will match ...
  //fallback and rely on info validity in these cases to determine more likely header.

  //a valid checksum is the biggest indicator of a valid header.
  if((checksum + complement) == 0xffff && (checksum != 0) && (complement != 0)) score += 4;

  if(addr == 0x007fc0 && mapper == 0x20) score += 2;  //0x20 is usually LoROM
  if(addr == 0x00ffc0 && mapper == 0x21) score += 2;  //0x21 is usually HiROM
  if(addr == 0x007fc0 && mapper == 0x22) score += 2;  //0x22 is usually ExLoROM
  if(addr == 0x40ffc0 && mapper == 0x25) score += 2;  //0x25 is usually ExHiROM

  if(data[addr + Company] == 0x33) score += 2;        //0x33 indicates extended header
  if(data[addr + RomType] < 0x08) score++;
  if(data[addr + RomSize] < 0x10) score++;
  if(data[addr + RamSize] < 0x08) score++;
  if(data[addr + CartRegion] < 14) score++;

  if(score < 0) score = 0;
  return score;
}
