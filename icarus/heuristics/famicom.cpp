struct FamicomCartridge {
  FamicomCartridge(const uint8_t* data, uint size);

  string markup;

//private:
  uint mapper;
  uint mirror;
  uint prgrom;
  uint prgram;
  uint chrrom;
  uint chrram;
};

FamicomCartridge::FamicomCartridge(const uint8_t* data, uint size) {
  if(size < 16) return;
  if(data[0] != 'N') return;
  if(data[1] != 'E') return;
  if(data[2] != 'S') return;
  if(data[3] !=  26) return;

  mapper = ((data[7] >> 4) << 4) | (data[6] >> 4);
  mirror = ((data[6] & 0x08) >> 2) | (data[6] & 0x01);
  prgrom = data[4] * 0x4000;
  chrrom = data[5] * 0x2000;
  prgram = 0u;
  chrram = chrrom == 0u ? 8192u : 0u;

  markup.append("board ");

  switch(mapper) {
  default:
    markup.append("id:NES-NROM-256\n");
    markup.append("  mirror mode=", mirror == 0 ? "horizontal" : "vertical", "\n");
    break;

  case   1:
    markup.append("id:NES-SXROM\n");
    markup.append("  chip type=MMC1B2\n");
    prgram = 8192;
    break;

  case   2:
    markup.append("id:NES-UOROM\n");
    markup.append("  mirror mode=", mirror == 0 ? "horizontal" : "vertical", "\n");
    break;

  case   3:
    markup.append("id:NES-CNROM\n");
    markup.append("  mirror mode=", mirror == 0 ? "horizontal" : "vertical", "\n");
    break;

  case   4:
    //MMC3
    markup.append("id:NES-TLROM\n");
    markup.append("  chip type=MMC3B\n");
    prgram = 8192;
    //MMC6
  //markup.append("id:NES-HKROM\n");
  //markup.append("  chip type=MMC6n");
  //prgram = 1024;
    break;

  case   5:
    markup.append("id:NES-ELROM\n");
    markup.append("  chip type=MMC5\n");
    prgram = 65536;
    break;

  case   7:
    markup.append("id:NES-AOROM\n");
    break;

  case   9:
    markup.append("id:NES-PNROM\n");
    markup.append("  chip type=MMC2\n");
    prgram = 8192;
    break;

  case  10:
    markup.append("id:NES-FKROM\n");
    markup.append("  chip type=MMC4\n");
    prgram = 8192;
    break;

  case  16:
    markup.append("id:BANDAI-FCG\n");
    markup.append("  chip type=LZ93D50\n");
    break;

  case  21:
  case  23:
  case  25:
    //VRC4
    markup.append("id:KONAMI-VRC-4\n");
    markup.append("  chip type=VRC4\n");
    markup.append("    pinout a0=1 a1=0\n");
    prgram = 8192;
    break;

  case  22:
    //VRC2
    markup.append("id:KONAMI-VRC-2\n");
    markup.append("  chip type=VRC2\n");
    markup.append("    pinout a0=0 a1=1\n");
    break;

  case  24:
    markup.append("id:KONAMI-VRC-6\n");
    markup.append("  chip type=VRC6\n");
    break;

  case  26:
    markup.append("id:KONAMI-VRC-6\n");
    markup.append("  chip type=VRC6\n");
    prgram = 8192;
    break;

  case  34:
    markup.append("id:NES-BNROM\n");
    markup.append("  mirror mode=", mirror == 0 ? "horizontal" : "vertical", "\n");
    break;

  case  66:
    markup.append("id:NES-GNROM\n");
    markup.append("  mirror mode=", mirror == 0 ? "horizontal" : "vertical", "\n");
    break;

  case  69:
    markup.append("id:SUNSOFT-5B\n");
    markup.append("  chip type=5B\n");
    prgram = 8192;
    break;

  case  73:
    markup.append("id:KONAMI-VRC-3\n");
    markup.append("  chip type=VRC3\n");
    markup.append("  mirror mode=", mirror == 0 ? "horizontal" : "vertical", "\n");
    prgram = 8192;
    break;

  case  75:
    markup.append("id:KONAMI-VRC-1\n");
    markup.append("  chip type=VRC1\n");
    break;

  case  85:
    markup.append("id:KONAMI-VRC-7\n");
    markup.append("  chip type=VRC7\n");
    prgram = 8192;
    break;
  }

  markup.append("  prg\n");
  if(prgrom) markup.append("    rom name=program.rom size=0x", hex(prgrom), "\n");
  if(prgram) markup.append("    ram name=save.ram size=0x", hex(prgram), "\n");

  markup.append("  chr\n");
  if(chrrom) markup.append("    rom name=character.rom size=0x", hex(chrrom), "\n");
  if(chrram) markup.append("    ram size=0x", hex(chrram), "\n");
}
