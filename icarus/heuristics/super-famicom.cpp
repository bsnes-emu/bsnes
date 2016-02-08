struct SuperFamicomCartridge {
  SuperFamicomCartridge(const uint8* data, uint size, bool has_msu1 = false);

  string markup;

//private:
  auto readHeader(const uint8* data, uint size) -> void;
  auto findHeader(const uint8* data, uint size) -> uint;
  auto scoreHeader(const uint8* data, uint size, uint addr) -> uint;

  enum HeaderField : uint {
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

  enum class Type : uint {
    SatellaviewBIOS,
    SufamiTurboBIOS,
    SuperGameBoy1BIOS,
    SuperGameBoy2BIOS,
    LoROM,
    HiROM,
    ExLoROM,
    ExHiROM,
    SuperFX,
    SA1,
    LoROMSatellaview,
    HiROMSatellaview,
    CampusChallenge92,
    Powerfest94,

    //invalid types
    Unknown,
    GameBoy,
    Satellaview,
    SufamiTurbo,
  };

  enum class Region : uint {
    NTSC,
    PAL,
  };

  enum class DSP1Type : uint {
    None,
    LoROM1MB,
    LoROM2MB,
    HiROM,
  };

  bool loaded = false;             //is a base cartridge inserted?
  uint crc32 = 0;                  //crc32 of all cartridges (base+slot(s))
  uint rom_size = 0;
  uint ram_size = 0;
  bool firmware_required = false;  //true if firmware is required for emulation
  bool firmware_appended = false;  //true if firmware is present at end of data

  Type type = Type::Unknown;
  Region region = Region::NTSC;
  DSP1Type dsp1_type = DSP1Type::None;

  bool has_bsx_slot = false;
  bool has_superfx = false;
  bool has_sa1 = false;
  bool has_sharprtc = false;
  bool has_epsonrtc = false;
  bool has_sdd1 = false;
  bool has_spc7110 = false;
  bool has_cx4 = false;
  bool has_dsp1 = false;
  bool has_dsp2 = false;
  bool has_dsp3 = false;
  bool has_dsp4 = false;
  bool has_obc1 = false;
  bool has_st010 = false;
  bool has_st011 = false;
  bool has_st018 = false;
};

SuperFamicomCartridge::SuperFamicomCartridge(const uint8* data, uint size, bool has_msu1) {
  //skip copier header
  if((size & 0x7fff) == 512) data += 512, size -= 512;

  if(size < 0x8000) return;

  readHeader(data, size);

  if(type == Type::Unknown) return;
  if(type == Type::GameBoy) return;
  if(type == Type::Satellaview) return;
  if(type == Type::SufamiTurbo) return;

  const char* range = (rom_size > 0x200000) || (ram_size > 32 * 1024) ? "0000-7fff" : "0000-ffff";
  markup.append("board region=", region == Region::NTSC ? "ntsc" : "pal", "\n");

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

  if(type == Type::SuperGameBoy1BIOS || type == Type::SuperGameBoy2BIOS) {
    firmware_required = true;
    if((rom_size & 0x7fff) == 0x100) {
      firmware_appended = true;
      rom_size -= 0x100;
    }
  }

  //end firmware detection

  if(type == Type::SatellaviewBIOS) {
    markup.append(
      "  ram name=save.ram size=0x", hex(ram_size), "\n"
      "    map address=10-1f:5000-5fff mask=0xf000\n"
      "  mcc\n"
      "    map address=00-0f:5000\n"
      "    map=mcu address=00-3f,80-bf:8000-ffff mask=0x408000\n"
      "    map=mcu address=40-7d,c0-ff:0000-ffff\n"
      "    rom name=program.rom size=0x", hex(rom_size), "\n"
      "    ram name=download.ram size=0x80000\n"
      "      map address=00-3f,80-bf:6000-7fff mask=0xe000\n"
      "    bsmemory\n"
    );
  }

  else if(type == Type::SufamiTurboBIOS) {
    markup.append(
      "  rom name=program.rom size=0x", hex(rom_size), "\n"
      "    map address=00-1f,80-9f:8000-ffff mask=0x8000\n"
      "  sufamiturbo\n"
      "    rom\n"
      "      map address=20-3f,a0-bf:8000-ffff mask=0x8000\n"
      "    ram\n"
      "      map address=60-6f,e0-ef:0000-ffff\n"
      "  sufamiturbo\n"
      "    rom\n"
      "      map address=40-5f,c0-df:0000-7fff mask=0x8000\n"
      "      map address=40-5f,c0-df:8000-ffff mask=0x8000\n"
      "    ram\n"
      "      map address=70-7d,f0-ff:0000-ffff\n"
    );
  }

  else if(type == Type::SuperGameBoy1BIOS || type == Type::SuperGameBoy2BIOS) {
    markup.append(
      "  rom name=program.rom size=0x", hex(rom_size), "\n"
      "    map address=00-7d,80-ff:8000-ffff mask=0x8000\n"
      "    map address=40-7d,c0-ff:0000-7fff mask=0x8000\n"
      "  icd2 revision=1\n"
      "    map address=00-3f,80-bf:6000-67ff,7000-7fff\n"
      "    rom name=sgb.boot.rom size=0x100\n"
    );
  }

  else if(has_cx4) {
    markup.append(
      "  hitachidsp model=HG51B169 frequency=20000000\n"
      "    map address=00-3f,80-bf:6c00-6fff,7c00-7fff\n"
      "    rom name=program.rom size=0x", hex(rom_size), "\n"
      "      map address=00-3f,80-bf:8000-ffff mask=0x8000\n"
      "    ram name=save.ram size=0\n"
      "      map address=70-77:0000-7fff mask=0x8000\n"
      "    drom name=cx4.data.rom size=0xc00\n"
      "    dram name=cx4.data.ram size=0xc00 volatile\n"
      "      map address=00-3f,80-bf:6000-6bff,7000-7bff mask=0xf000\n"
    );
  }

  else if(has_spc7110) {
    markup.append(
      "  spc7110\n"
      "    map address=00-3f,80-bf:4800-483f\n"
      "    map address=50,58:0000-ffff\n"
      "    map=mcu address=00-3f,80-bf:8000-ffff mask=0x800000\n"
      "    map=mcu address=c0-ff:0000-ffff mask=0xc00000\n"
      "    prom name=program.rom size=0x100000\n"
      "    drom name=data.rom size=0x", hex(rom_size - 0x100000), "\n"
      "    ram name=save.ram size=0x", hex(ram_size), "\n"
      "      map address=00-3f,80-bf:6000-7fff mask=0xe000\n"
    );
  }

  else if(has_sdd1) {
    markup.append(
      "  sdd1\n"
      "    map address=00-3f,80-bf:4800-4807\n"
      "    rom name=program.rom size=0x", hex(rom_size), "\n"
      "      map address=00-3f,80-bf:8000-ffff mask=0x808000\n"
      "      map address=c0-ff:0000-ffff\n"
    );
    if(ram_size > 0) markup.append(
      "    ram name=save.ram size=0x", hex(ram_size), "\n"
      "      map address=20-3f,a0-bf:6000-7fff mask=0xe000\n"
      "      map address=70-7d:0000-7fff mask=0x8000\n"
    );
  }

  else if(type == Type::LoROM) {
    markup.append(
      "  rom name=program.rom size=0x", hex(rom_size), "\n"
      "    map address=00-7d,80-ff:8000-ffff mask=0x8000\n"
      "    map address=40-6f,c0-ef:0000-7fff mask=0x8000\n"
    );
    if(ram_size > 0) markup.append(
      "  ram name=save.ram size=0x", hex(ram_size), "\n"
      "    map address=70-7d,f0-ff:", range, "\n"
    );
  }

  else if(type == Type::HiROM) {
    markup.append(
      "  rom name=program.rom size=0x", hex(rom_size), "\n"
      "    map address=00-3f,80-bf:8000-ffff\n"
      "    map address=40-7f,c0-ff:0000-ffff\n"
    );
    if(ram_size > 0) markup.append(
      "  ram name=save.ram size=0x", hex(ram_size), "\n"
      "    map address=10-3f,90-bf:6000-7fff mask=0xe000\n"
    );
  }

  else if(type == Type::ExLoROM) {
    markup.append(
      "  rom name=program.rom size=0x", hex(rom_size), "\n"
      "    map address=00-3f,80-bf:8000-ffff mask=0x8000\n"
      "    map address=40-7d:0000-ffff\n"
    );
    if(ram_size > 0) markup.append(
      "  ram name=save.ram size=0x", hex(ram_size), "\n"
      "    map address=20-3f,a0-bf:6000-7fff mask=0xe000\n"
      "    map address=70-7d:0000-7fff mask=0x8000\n"
    );
  }

  else if(type == Type::ExHiROM) {
    markup.append(
      "  rom name=program.rom size=0x", hex(rom_size), "\n"
      "    map address=00-3f:8000-ffff base=0x400000\n"
      "    map address=40-7d:0000-ffff base=0x400000\n"
      "    map address=80-bf:8000-ffff mask=0xc00000\n"
      "    map address=c0-ff:0000-ffff mask=0xc00000\n"
    );
    if(ram_size > 0) markup.append(
      "  ram name=save.ram size=0x", hex(ram_size), "\n"
      "    map address=20-3f,a0-bf:6000-7fff mask=0xe000\n"
      "    map address=70-7d:", range, "\n"
    );
  }

  else if(type == Type::SuperFX) {
    markup.append(
      "  superfx\n"
      "    map address=00-3f,80-bf:3000-34ff\n"
      "    rom name=program.rom size=0x", hex(rom_size), "\n"
      "      map address=00-3f,80-bf:8000-ffff mask=0x8000\n"
      "      map address=40-5f,c0-df:0000-ffff\n"
    );
    if(ram_size > 0) markup.append(
      "    ram name=save.ram size=0x", hex(ram_size), "\n"
      "      map address=00-3f,80-bf:6000-7fff size=0x2000\n"
      "      map address=70-71,f0-f1:0000-ffff\n"
    );
  }

  else if(type == Type::SA1) {
    markup.append(
      "  sa1\n"
      "    map address=00-3f,80-bf:2200-23ff\n"
      "    rom name=program.rom size=0x", hex(rom_size), "\n"
      "      map address=00-3f,80-bf:8000-ffff mask=0x408000\n"
      "      map address=c0-ff:0000-ffff\n"
    );
    if(ram_size > 0) markup.append(
      "    bwram name=save.ram size=0x", hex(ram_size), "\n"
      "      map address=00-3f,80-bf:6000-7fff size=0x2000\n"
      "      map address=40-4f:0000-ffff\n"
    );
    markup.append(
      "    iram id=internal size=0x800 volatile\n"
      "      map address=00-3f,80-bf:3000-37ff size=0x800\n"
    );
  }

  else if(type == Type::LoROMSatellaview) {
    markup.append(
      "  rom name=program.rom size=0x", hex(rom_size), "\n"
      "    map address=00-1f:8000-ffff base=0x000000 mask=0x8000\n"
      "    map address=20-3f:8000-ffff base=0x100000 mask=0x8000\n"
      "    map address=80-9f:8000-ffff base=0x200000 mask=0x8000\n"
      "    map address=a0-bf:8000-ffff base=0x100000 mask=0x8000\n"
      "  ram name=save.ram size=0x", hex(ram_size), "\n"
      "    map address=70-7d,f0-ff:0000-7fff mask=0x8000\n"
      "  bsmemory\n"
      "    map address=c0-ef:0000-ffff\n"
    );
  }

  else if(type == Type::HiROMSatellaview) {
    markup.append(
      "  rom name=program.rom size=0x", hex(rom_size), "\n"
      "    map address=00-1f,80-9f:8000-ffff\n"
      "    map address=40-5f,c0-df:0000-ffff\n"
      "  ram name=save.ram size=0x", hex(ram_size), "\n"
      "    map address=20-3f,a0-bf:6000-7fff\n"
      "  bsmemory\n"
      "    map address=20-3f,a0-bf:8000-ffff\n"
      "    map address=60-7f,e0-ff:0000-ffff\n"
    );
  }

  else if(type == Type::CampusChallenge92) {
    markup.append(
      "  event=CC92 timer=360\n"
      "    map address=c0,e0:0000\n"
      "    map=mcu address=00-1f,80-9f:8000-ffff\n"
      "    rom name=program.rom size=0x40000\n"
      "    rom name=slot-1.rom size=0x80000\n"
      "    rom name=slot-2.rom size=0x80000\n"
      "    rom name=slot-3.rom size=0x80000\n"
      "    ram name=save.ram size=0x2000 volatile\n"
      "      map address=70-7d,f0-ff:0000-7fff mask=0x8000\n"
      "  necdsp model=uPD7725 frequency=8000000\n"
      "    map address=20-3f,a0-bf:8000-ffff mask=0x7fff\n"
      "    prom name=dsp1.program.rom size=0x1800\n"
      "    drom name=dsp1.data.rom size=0x800\n"
      "    dram name=dsp1.data.ram size=0x200 volatile\n"
    );
    return;
  }

  else if(type == Type::Powerfest94) {
    markup.append(
      "  event=PF94 timer=360\n"
      "    map address=10,20:6000\n"
      "    map=mcu address=00-3f,80-bf:8000-ffff\n"
      "    map=mcu address=c0-ff:0000-ffff\n"
      "    rom name=program.rom size=0x40000\n"
      "    rom name=slot-1.rom size=0x80000\n"
      "    rom name=slot-2.rom size=0x80000\n"
      "    rom name=slot-3.rom size=0x100000\n"
      "    ram name=save.ram size=0x2000 volatile\n"
      "      map address=30-3f,b0-bf:6000-7fff mask=0xe000\n"
      "  necdsp model=uPD7725 frequency=8000000\n"
      "    map address=00-0f,80-8f:6000-7fff mask=0xfff\n"
      "    prom name=dsp1.program.rom size=0x1800\n"
      "    drom name=dsp1.data.rom size=0x800\n"
      "    dram name=dsp1.data.ram size=0x200 volatile\n"
    );
    return;
  }

  if(has_sharprtc) {
    markup.append(
      "  sharprtc\n"
      "    map address=00-3f,80-bf:2800-2801\n"
      "    ram name=rtc.ram size=0x10\n"
    );
  }

  if(has_epsonrtc) {
    markup.append(
      "  epsonrtc\n"
      "    map address=00-3f,80-bf:4840-4842\n"
      "    ram name=rtc.ram size=0x10\n"
    );
  }

  if(has_obc1) {
    markup.append(
      "  obc1\n"
      "    map address=00-3f,80-bf:6000-7fff mask=0xe000\n"
      "    ram name=save.ram size=0x2000\n"
    );
  }

  if(has_dsp1) {
    markup.append(
      "  necdsp model=uPD7725 frequency=8000000\n"
    );
    if(dsp1_type == DSP1Type::LoROM1MB) markup.append(
      "    map address=20-3f,a0-bf:8000-ffff mask=0x3fff\n"
    );
    if(dsp1_type == DSP1Type::LoROM2MB) markup.append(
      "    map address=60-6f,e0-ef:0000-7fff mask=0x3fff\n"
    );
    if(dsp1_type == DSP1Type::HiROM) markup.append(
      "    map address=00-1f,80-9f:6000-7fff mask=0xfff\n"
    );
    markup.append(
      "    prom name=dsp1b.program.rom size=0x1800\n"
      "    drom name=dsp1b.data.rom size=0x800\n"
      "    dram name=dsp1b.data.ram size=0x200 volatile\n"
    );
  }

  if(has_dsp2) {
    markup.append(
      "  necdsp model=uPD7725 frequency=8000000\n"
      "    map address=20-3f,a0-bf:8000-ffff mask=0x3fff\n"
      "    prom name=dsp2.program.rom size=0x1800\n"
      "    drom name=dsp2.data.rom size=0x800\n"
      "    dram name=dsp2.data.ram size=0x200 volatile\n"
    );
  }

  if(has_dsp3) {
    markup.append(
      "  necdsp model=uPD7725 frequency=8000000\n"
      "    map address=20-3f,a0-bf:8000-ffff mask=0x3fff\n"
      "    prom name=dsp3.program.rom size=0x1800\n"
      "    drom name=dsp3.data.rom size=0x800\n"
      "    dram name=dsp3.data.ram size=0x200 volatile\n"
    );
  }

  if(has_dsp4) {
    markup.append(
      "  necdsp model=uPD7725 frequency=8000000\n"
      "    map address=30-3f,b0-bf:8000-ffff mask=0x3fff\n"
      "    prom name=dsp4.program.rom size=0x1800\n"
      "    drom name=dsp4.data.rom size=0x800\n"
      "    dram name=dsp4.data.ram size=0x200 volatile\n"
    );
  }

  if(has_st010) {
    markup.append(
      "  necdsp model=uPD96050 frequency=11000000\n"
      "    map address=60-67,e0-e7:0000-3fff\n"
      "    prom name=st010.program.rom size=0xc000\n"
      "    drom name=st010.data.rom size=0x1000\n"
      "    dram name=save.ram size=0x1000\n"
      "      map address=68-6f,e8-ef:0000-7fff mask=0x8000\n"
    );
  }

  if(has_st011) {
    markup.append(
      "  necdsp model=uPD96050 frequency=15000000\n"
      "    map address=60-67,e0-e7:0000-3fff\n"
      "    prom name=st011.program.rom size=0xc000\n"
      "    drom name=st011.data.rom size=0x1000\n"
      "    dram name=save.ram size=0x1000\n"
      "      map address=68-6f,e8-ef:0000-7fff mask=0x8000\n"
    );
  }

  if(has_st018) {
    markup.append(
      "  armdsp frequency=21477272\n"
      "    map address=00-3f,80-bf:3800-38ff\n"
      "    prom name=st018.program.rom size=0x20000\n"
      "    drom name=st018.data.rom size=0x8000\n"
      "    dram name=save.ram size=0x4000\n"
    );
  }

  if(has_msu1) {
    markup.append(
      "  msu1\n"
      "    map address=00-3f,80-bf:2000-2007\n"
      "    rom name=msu1.rom\n"
    );
  }
}

auto SuperFamicomCartridge::readHeader(const uint8* data, uint size) -> void {
  //detect Game Boy carts
  if(size >= 0x0140) {
    if(data[0x0104] == 0xce && data[0x0105] == 0xed && data[0x0106] == 0x66 && data[0x0107] == 0x66
    && data[0x0108] == 0xcc && data[0x0109] == 0x0d && data[0x010a] == 0x00 && data[0x010b] == 0x0b) {
      type = Type::GameBoy;
      return;
    }
  }

  const uint index = findHeader(data, size);
  const uint8 mapperid = data[index + Mapper];
  const uint8 rom_type = data[index + RomType];
  const uint8 rom_size = data[index + RomSize];
  const uint8 company  = data[index + Company];
  const uint8 regionid = data[index + CartRegion] & 0x7f;

  const uint16 complement = data[index + Complement] | data[index + Complement + 1] << 8;
  const uint16 checksum   = data[index + Checksum] | data[index + Checksum + 1] << 8;

  this->rom_size = size;
  ram_size = 1024 << (data[index + RamSize] & 7);
  if(ram_size == 1024) ram_size = 0;  //no RAM present
  if(rom_size == 0 && ram_size) ram_size = 0;  //fix for Bazooka Blitzkrieg's malformed header (swapped ROM and RAM sizes)

  //0, 1, 13 = NTSC; 2 - 12 = PAL
  region = (regionid <= 1 || regionid >= 13) ? Region::NTSC : Region::PAL;

  //detect BS-X flash carts
  if(data[index + 0x13] == 0x00 || data[index + 0x13] == 0xff) {
    if(data[index + 0x14] == 0x00) {
      const uint8 n15 = data[index + 0x15];
      if(n15 == 0x00 || n15 == 0x80 || n15 == 0x84 || n15 == 0x9c || n15 == 0xbc || n15 == 0xfc) {
        if(data[index + 0x1a] == 0x33 || data[index + 0x1a] == 0xff) {
          type = Type::Satellaview;
          region = Region::NTSC;  //BS-X only released in Japan
          return;
        }
      }
    }
  }

  //detect Sufami Turbo carts
  if(!memcmp(data, "BANDAI SFC-ADX", 14)) {
    if(!memcmp(data + 16, "SFC-ADX BACKUP", 14)) {
      type = Type::SufamiTurboBIOS;
    } else {
      type = Type::SufamiTurbo;
    }
    region = Region::NTSC;  //Sufami Turbo only released in Japan
    return;                 //RAM size handled outside this routine
  }

  //detect Super Game Boy BIOS
  if(!memcmp(data + index, "Super GAMEBOY2", 14)) {
    type = Type::SuperGameBoy2BIOS;
    return;
  }

  if(!memcmp(data + index, "Super GAMEBOY", 13)) {
    type = Type::SuperGameBoy1BIOS;
    return;
  }

  //detect competition carts
  if(!memcmp(data + index, "\x00\x08\x22\x02\x1c\x00\x10\x00\x08\x65\x80\x84\x20\x00\x22\x25\x00\x83\x0c\x80\x10", 21)
  && complement == 0x0100 && checksum == 0x2d02 && (size == 0x1c0000 || size == 0x1c2000)) {
    type = Type::CampusChallenge92;  //dark title screen version
    return;
  }

  if(!memcmp(data + index, "\xc9\x80\x80\x44\x15\x00\x62\x09\x29\xa0\x52\x70\x50\x12\x05\x35\x31\x63\xc0\x22\x01", 21)
  && complement == 0x2011 && checksum == 0xf8c0 && (size == 0x240000 || size == 0x242000)) {
    type = Type::Powerfest94;  //10,000 points version
    return;
  }

  if(!memcmp(data + index, "PREHISTORIK MAN      ", 21)
  && complement == 0xffff && checksum == 0x0000 && (size == 0x240000 || size == 0x242000)) {
    type = Type::Powerfest94;  //1,000,000 points version
    return;
  }

  //detect presence of BS-X flash cartridge connector (reads extended header information)
  if(data[index - 14] == 'Z') {
    if(data[index - 11] == 'J') {
      uint8 n13 = data[index - 13];
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
      type = Type::SatellaviewBIOS;
      region = Region::NTSC;  //BS-X only released in Japan
      return;                 //RAM size handled internally by load_cart_bsx() -> BSXCart class
    } else {
      type = (index == 0x7fc0 ? Type::LoROMSatellaview : Type::HiROMSatellaview);
      region = Region::NTSC;  //BS-X slotted cartridges only released in Japan
    }
  } else {
    //standard cart
    if(index == 0x7fc0 && size >= 0x401000) {
      type = Type::ExLoROM;
    } else if(index == 0x7fc0 && mapperid == 0x32) {
      type = Type::ExLoROM;
    } else if(index == 0x7fc0) {
      type = Type::LoROM;
    } else if(index == 0xffc0) {
      type = Type::HiROM;
    } else if(index == 0x40ffc0) {
      type = Type::ExHiROM;
    } else {
      type = Type::Unknown;
      return;
    }
  }

  if(mapperid == 0x20 && (rom_type == 0x13 || rom_type == 0x14 || rom_type == 0x15 || rom_type == 0x1a)) {
    has_superfx = true;
    type = Type::SuperFX;
    ram_size = 1024 << (data[index - 3] & 7);
    if(ram_size == 1024) ram_size = 0;
  }

  if(mapperid == 0x23 && (rom_type == 0x32 || rom_type == 0x34 || rom_type == 0x35)) {
    has_sa1 = true;
    type = Type::SA1;
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
      dsp1_type = DSP1Type::LoROM1MB;
    } else if((mapperid & 0x2f) == 0x20) {
      dsp1_type = DSP1Type::LoROM2MB;
    } else if((mapperid & 0x2f) == 0x21) {
      dsp1_type = DSP1Type::HiROM;
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

auto SuperFamicomCartridge::findHeader(const uint8* data, uint size) -> uint {
  uint score_lo = scoreHeader(data, size, 0x007fc0);
  uint score_hi = scoreHeader(data, size, 0x00ffc0);
  uint score_ex = scoreHeader(data, size, 0x40ffc0);
  if(score_ex) score_ex += 4;  //favor ExHiROM on images > 32mbits

  if(score_lo >= score_hi && score_lo >= score_ex) {
    return 0x007fc0;
  } else if(score_hi >= score_ex) {
    return 0x00ffc0;
  } else {
    return 0x40ffc0;
  }
}

auto SuperFamicomCartridge::scoreHeader(const uint8* data, uint size, uint addr) -> uint {
  if(size < addr + 64) return 0;  //image too small to contain header at this location?
  int score = 0;

  uint16 resetvector = data[addr + ResetVector] | (data[addr + ResetVector + 1] << 8);
  uint16 checksum    = data[addr + Checksum   ] | (data[addr + Checksum    + 1] << 8);
  uint16 complement  = data[addr + Complement ] | (data[addr + Complement  + 1] << 8);

  uint8 resetop = data[(addr & ~0x7fff) | (resetvector & 0x7fff)];  //first opcode executed upon reset
  uint8 mapper  = data[addr + Mapper] & ~0x10;                      //mask off irrelevent FastROM-capable bit

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
