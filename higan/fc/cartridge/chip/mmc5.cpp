struct MMC5 : Chip {
  MMC5(Board& board) : Chip(board) {
    revision = Revision::MMC5;
  }

  auto main() -> void {
    //scanline() resets this; if no scanlines detected, enter video blanking period
    if(++cpuCycleCounter >= 200) blank();  //113-114 normal; ~2500 across Vblank period

    cpu.irqLine(irqEnable && irqPending);
    tick();
  }

  auto scanline(uint y) -> void {
  //used for testing only, to verify MMC5 scanline detection is accurate:
  //if(y != vcounter && y <= 240) print(y, " vs ", vcounter, "\n");
  }

  auto accessPRG(bool write, uint addr, uint8 data = 0x00) -> uint8 {
    uint bank;

    if((addr & 0xe000) == 0x6000) {
      bank = (ramSelect << 2) | ramBank;
      addr &= 0x1fff;
    } else if(prgMode == 0) {
      bank = prgBank[3] & ~3;
      addr &= 0x7fff;
    } else if(prgMode == 1) {
      if((addr & 0xc000) == 0x8000) bank = (prgBank[1] & ~1);
      if((addr & 0xe000) == 0xc000) bank = (prgBank[3] & ~1);
      addr &= 0x3fff;
    } else if(prgMode == 2) {
      if((addr & 0xe000) == 0x8000) bank = (prgBank[1] & ~1) | 0;
      if((addr & 0xe000) == 0xa000) bank = (prgBank[1] & ~1) | 1;
      if((addr & 0xe000) == 0xc000) bank = (prgBank[2]);
      if((addr & 0xe000) == 0xe000) bank = (prgBank[3]);
      addr &= 0x1fff;
    } else if(prgMode == 3) {
      if((addr & 0xe000) == 0x8000) bank = prgBank[0];
      if((addr & 0xe000) == 0xa000) bank = prgBank[1];
      if((addr & 0xe000) == 0xc000) bank = prgBank[2];
      if((addr & 0xe000) == 0xe000) bank = prgBank[3];
      addr &= 0x1fff;
    }

    bool rom = bank & 0x80;
    bank &= 0x7f;

    if(write == false) {
      if(rom) {
        return board.prgrom.read((bank << 13) | addr);
      } else {
        return board.prgram.read((bank << 13) | addr);
      }
    } else {
      if(rom) {
        board.prgrom.write((bank << 13) | addr, data);
      } else {
        if(prgramWriteProtect[0] == 2 && prgramWriteProtect[1] == 1) {
          board.prgram.write((bank << 13) | addr, data);
        }
      }
      return 0x00;
    }
  }

  auto readPRG(uint addr) -> uint8 {
    if((addr & 0xfc00) == 0x5c00) {
      if(exramMode >= 2) return exram[addr & 0x03ff];
      return cpu.mdr();
    }

    if(addr >= 0x6000) {
      return accessPRG(0, addr);
    }

    switch(addr) {
    case 0x5204: {
      uint8 result = (irqPending << 7) | (inFrame << 6);
      irqPending = false;
      return result;
    }
    case 0x5205: return (multiplier * multiplicand) >> 0;
    case 0x5206: return (multiplier * multiplicand) >> 8;
    }
  }

  auto writePRG(uint addr, uint8 data) -> void {
    if((addr & 0xfc00) == 0x5c00) {
      //writes 0x00 *during* Vblank (not during screen rendering ...)
      if(exramMode == 0 || exramMode == 1) exram[addr & 0x03ff] = inFrame ? data : (uint8)0x00;
      if(exramMode == 2) exram[addr & 0x03ff] = data;
      return;
    }

    if(addr >= 0x6000) {
      accessPRG(1, addr, data);
      return;
    }

    switch(addr) {
    case 0x2000:
      sprite8x16 = data & 0x20;
      break;

    case 0x2001:
      //if BG+sprites are disabled; enter video blanking period
      if((data & 0x18) == 0) blank();
      break;

    case 0x5100: prgMode = data & 3; break;
    case 0x5101: chrMode = data & 3; break;

    case 0x5102: prgramWriteProtect[0] = data & 3; break;
    case 0x5103: prgramWriteProtect[1] = data & 3; break;

    case 0x5104:
      exramMode = data & 3;
      break;

    case 0x5105:
      nametableMode[0] = (data & 0x03) >> 0;
      nametableMode[1] = (data & 0x0c) >> 2;
      nametableMode[2] = (data & 0x30) >> 4;
      nametableMode[3] = (data & 0xc0) >> 6;
      break;

    case 0x5106:
      fillmodeTile = data;
      break;

    case 0x5107:
      fillmodeColor = data & 3;
      fillmodeColor |= fillmodeColor << 2;
      fillmodeColor |= fillmodeColor << 4;
      break;

    case 0x5113:
      ramSelect = data & 0x04;
      ramBank = data & 0x03;
      break;

    case 0x5114: prgBank[0] = data; break;
    case 0x5115: prgBank[1] = data; break;
    case 0x5116: prgBank[2] = data; break;
    case 0x5117: prgBank[3] = data | 0x80; break;

    case 0x5120: chrSpriteBank[0] = (chrBankHi << 8) | data; chrActive = 0; break;
    case 0x5121: chrSpriteBank[1] = (chrBankHi << 8) | data; chrActive = 0; break;
    case 0x5122: chrSpriteBank[2] = (chrBankHi << 8) | data; chrActive = 0; break;
    case 0x5123: chrSpriteBank[3] = (chrBankHi << 8) | data; chrActive = 0; break;
    case 0x5124: chrSpriteBank[4] = (chrBankHi << 8) | data; chrActive = 0; break;
    case 0x5125: chrSpriteBank[5] = (chrBankHi << 8) | data; chrActive = 0; break;
    case 0x5126: chrSpriteBank[6] = (chrBankHi << 8) | data; chrActive = 0; break;
    case 0x5127: chrSpriteBank[7] = (chrBankHi << 8) | data; chrActive = 0; break;

    case 0x5128: chrBGBank[0] = (chrBankHi << 8) | data; chrActive = 1; break;
    case 0x5129: chrBGBank[1] = (chrBankHi << 8) | data; chrActive = 1; break;
    case 0x512a: chrBGBank[2] = (chrBankHi << 8) | data; chrActive = 1; break;
    case 0x512b: chrBGBank[3] = (chrBankHi << 8) | data; chrActive = 1; break;

    case 0x5130:
      chrBankHi = data & 3;
      break;

    case 0x5200:
      vsEnable = data & 0x80;
      vsSide = data & 0x40;
      vsTile = data & 0x1f;
      break;

    case 0x5201:
      vsScroll = data;
      break;

    case 0x5202:
      vsBank = data;
      break;

    case 0x5203:
      irqLine = data;
      break;

    case 0x5204:
      irqEnable = data & 0x80;
      break;

    case 0x5205:
      multiplicand = data;
      break;

    case 0x5206:
      multiplier = data;
      break;
    }
  }

  auto chrSpriteAddr(uint addr) -> uint {
    if(chrMode == 0) {
      auto bank = chrSpriteBank[7];
      return (bank * 0x2000) + (addr & 0x1fff);
    }

    if(chrMode == 1) {
      auto bank = chrSpriteBank[(addr / 0x1000) * 4 + 3];
      return (bank * 0x1000) + (addr & 0x0fff);
    }

    if(chrMode == 2) {
      auto bank = chrSpriteBank[(addr / 0x0800) * 2 + 1];
      return (bank * 0x0800) + (addr & 0x07ff);
    }

    if(chrMode == 3) {
      auto bank = chrSpriteBank[(addr / 0x0400)];
      return (bank * 0x0400) + (addr & 0x03ff);
    }
  }

  auto chrBGAddr(uint addr) -> uint {
    addr &= 0x0fff;

    if(chrMode == 0) {
      auto bank = chrBGBank[3];
      return (bank * 0x2000) + (addr & 0x0fff);
    }

    if(chrMode == 1) {
      auto bank = chrBGBank[3];
      return (bank * 0x1000) + (addr & 0x0fff);
    }

    if(chrMode == 2) {
      auto bank = chrBGBank[(addr / 0x0800) * 2 + 1];
      return (bank * 0x0800) + (addr & 0x07ff);
    }

    if(chrMode == 3) {
      auto bank = chrBGBank[(addr / 0x0400)];
      return (bank * 0x0400) + (addr & 0x03ff);
    }
  }

  auto chrVSAddr(uint addr) -> uint {
    return (vsBank * 0x1000) + (addr & 0x0ff8) + (vsVpos & 7);
  }

  auto blank() -> void {
    inFrame = false;
  }

  auto scanline() -> void {
    hcounter = 0;

    if(inFrame == false) {
      inFrame = true;
      irqPending = false;
      vcounter = 0;
    } else {
      if(vcounter == irqLine) irqPending = true;
      vcounter++;
    }

    cpuCycleCounter = 0;
  }

  auto readCIRAM(uint addr) -> uint8 {
    if(vsFetch && (hcounter & 2) == 0) return exram[vsVpos / 8 * 32 + vsHpos / 8];
    if(vsFetch && (hcounter & 2) != 0) return exram[vsVpos / 32 * 8 + vsHpos / 32 + 0x03c0];

    switch(nametableMode[(addr >> 10) & 3]) {
    case 0: return ppu.readCIRAM(0x0000 | (addr & 0x03ff));
    case 1: return ppu.readCIRAM(0x0400 | (addr & 0x03ff));
    case 2: return exramMode < 2 ? exram[addr & 0x03ff] : (uint8)0x00;
    case 3: return (hcounter & 2) == 0 ? fillmodeTile : fillmodeColor;
    }
  }

  auto readCHR(uint addr) -> uint8 {
    chrAccess[0] = chrAccess[1];
    chrAccess[1] = chrAccess[2];
    chrAccess[2] = chrAccess[3];
    chrAccess[3] = addr;

    //detect two unused nametable fetches at end of each scanline
    if((chrAccess[0] & 0x2000) == 0
    && (chrAccess[1] & 0x2000)
    && (chrAccess[2] & 0x2000)
    && (chrAccess[3] & 0x2000)) scanline();

    if(inFrame == false) {
      vsFetch = false;
      if(addr & 0x2000) return readCIRAM(addr);
      return board.chrrom.read(chrActive ? chrBGAddr(addr) : chrSpriteAddr(addr));
    }

    bool bgFetch = (hcounter < 256 || hcounter >= 320);
    uint8 result = 0x00;

    if((hcounter & 7) == 0) {
      vsHpos  = hcounter >= 320 ? hcounter - 320 : hcounter + 16;
      vsVpos  = vcounter + vsScroll;
      vsFetch = vsEnable && bgFetch && exramMode < 2
      && (vsSide ? vsHpos / 8 >= vsTile : vsHpos / 8 < vsTile);
      if(vsVpos >= 240) vsVpos -= 240;

      result = readCIRAM(addr);

      exbank = (chrBankHi << 6) | (exram[addr & 0x03ff] & 0x3f);
      exattr = exram[addr & 0x03ff] >> 6;
      exattr |= exattr << 2;
      exattr |= exattr << 4;
    } else if((hcounter & 7) == 2) {
      result = readCIRAM(addr);
      if(bgFetch && exramMode == 1) result = exattr;
    } else {
      if(vsFetch) result = board.chrrom.read(chrVSAddr(addr));
      else if(sprite8x16 ? bgFetch : chrActive) result = board.chrrom.read(chrBGAddr(addr));
      else result = board.chrrom.read(chrSpriteAddr(addr));
      if(bgFetch && exramMode == 1) result = board.chrrom.read(exbank * 0x1000 + (addr & 0x0fff));
    }

    hcounter += 2;
    return result;
  }

  auto writeCHR(uint addr, uint8 data) -> void {
    if(addr & 0x2000) {
      switch(nametableMode[(addr >> 10) & 3]) {
      case 0: return ppu.writeCIRAM(0x0000 | (addr & 0x03ff), data);
      case 1: return ppu.writeCIRAM(0x0400 | (addr & 0x03ff), data);
      case 2: exram[addr & 0x03ff] = data; break;
      }
    }
  }

  auto power() -> void {
    for(auto& n : exram) n = 0xff;

    prgMode = 3;
    chrMode = 0;
    for(auto& n : prgramWriteProtect) n = 0;
    exramMode = 0;
    for(auto& n : nametableMode) n = 0;
    fillmodeTile = 0;
    fillmodeColor = 0;
    ramSelect = 0;
    ramBank = 0;
    prgBank[0] = 0x00;
    prgBank[1] = 0x00;
    prgBank[2] = 0x00;
    prgBank[3] = 0xff;
    for(auto& n : chrSpriteBank) n = 0;
    for(auto& n : chrBGBank) n = 0;
    chrBankHi = 0;
    vsEnable = 0;
    vsSide = 0;
    vsTile = 0;
    vsScroll = 0;
    vsBank = 0;
    irqLine = 0;
    irqEnable = 0;
    multiplicand = 0;
    multiplier = 0;

    cpuCycleCounter = 0;
    irqCounter = 0;
    irqPending = 0;
    inFrame = 0;
    vcounter = 0;
    hcounter = 0;
    for(auto& n : chrAccess) n = 0;
    chrActive = 0;
    sprite8x16 = 0;

    exbank = 0;
    exattr = 0;

    vsFetch = 0;
    vsVpos = 0;
    vsHpos = 0;
  }

  auto serialize(serializer& s) -> void {
    s.array(exram);

    s.integer(prgMode);
    s.integer(chrMode);
    for(auto& n : prgramWriteProtect) s.integer(n);
    s.integer(exramMode);
    for(auto& n : nametableMode) s.integer(n);
    s.integer(fillmodeTile);
    s.integer(fillmodeColor);
    s.integer(ramSelect);
    s.integer(ramBank);
    for(auto& n : prgBank) s.integer(n);
    for(auto& n : chrSpriteBank) s.integer(n);
    for(auto& n : chrBGBank) s.integer(n);
    s.integer(chrBankHi);
    s.integer(vsEnable);
    s.integer(vsSide);
    s.integer(vsTile);
    s.integer(vsScroll);
    s.integer(vsBank);
    s.integer(irqLine);
    s.integer(irqEnable);
    s.integer(multiplicand);
    s.integer(multiplier);

    s.integer(cpuCycleCounter);
    s.integer(irqCounter);
    s.integer(irqPending);
    s.integer(inFrame);

    s.integer(vcounter);
    s.integer(hcounter);
    for(auto& n : chrAccess) s.integer(n);
    s.integer(chrActive);
    s.integer(sprite8x16);

    s.integer(exbank);
    s.integer(exattr);

    s.integer(vsFetch);
    s.integer(vsVpos);
    s.integer(vsHpos);
  }

  enum class Revision : uint {
    MMC5,
    MMC5B,
  } revision;

  uint8 exram[1024];

  //programmable registers

  uint2 prgMode;  //$5100
  uint2 chrMode;  //$5101

  uint2 prgramWriteProtect[2];  //$5102,$5103

  uint2 exramMode;         //$5104
  uint2 nametableMode[4];  //$5105
  uint8 fillmodeTile;      //$5106
  uint8 fillmodeColor;     //$5107

  bool ramSelect;           //$5113
  uint2 ramBank;            //$5113
  uint8 prgBank[4];         //$5114-5117
  uint10 chrSpriteBank[8];  //$5120-5127
  uint10 chrBGBank[4];      //$5128-512b
  uint2 chrBankHi;          //$5130

  bool vsEnable;      //$5200
  bool vsSide;        //$5200
  uint5 vsTile;       //$5200
  uint8 vsScroll;     //$5201
  uint8 vsBank;       //$5202

  uint8 irqLine;      //$5203
  bool irqEnable;     //$5204

  uint8 multiplicand;  //$5205
  uint8 multiplier;    //$5206

  //status registers

  uint cpuCycleCounter;
  uint irqCounter;
  bool irqPending;
  bool inFrame;

  uint vcounter;
  uint hcounter;
  uint16 chrAccess[4];
  bool chrActive;
  bool sprite8x16;

  uint8 exbank;
  uint8 exattr;

  bool vsFetch;
  uint8 vsVpos;
  uint8 vsHpos;
};
