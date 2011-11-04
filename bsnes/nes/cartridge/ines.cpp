static string iNES(const uint8_t *data, unsigned size) {
  if(size < 16) return "";
  if(data[0] != 'N') return "";
  if(data[1] != 'E') return "";
  if(data[2] != 'S') return "";
  if(data[3] != 0x1a) return "";

  string output;

  unsigned mapper = ((data[7] >> 4) << 4) | (data[6] >> 4);
  unsigned mirror = ((data[6] & 0x08) >> 2) | (data[6] & 0x01);
  unsigned prgrom = data[4] * 0x4000;
  unsigned chrrom = data[5] * 0x2000;
  unsigned prgram = 0;
  unsigned chrram = chrrom == 0 ? 8192 : 0;

  print("iNES mapper: ", mapper, "\n");

  output.append("cartridge\n");

  switch(mapper) {
  default:
    output.append("\tboard type:NES-NROM-256\n");
    output.append("\t\tmirror:", mirror == 0 ? "horizontal" : "vertical", "\n");
    break;

  case   1:
    output.append("\tboard type:NES-SXROM\n");
    output.append("\t\tchip type:MMC1B2\n");
    prgram = 8192;
    break;

  case   2:
    output.append("\tboard type:NES-UOROM\n");
    output.append("\t\tmirror:", mirror == 0 ? "horizontal" : "vertical", "\n");
    break;

  case   3:
    output.append("\tboard type:NES-CNROM\n");
    output.append("\t\tmirror:", mirror == 0 ? "horizontal" : "vertical", "\n");
    break;

  case   4:
    //MMC3
    output.append("\tboard type:NES-TLROM\n");
    output.append("\t\tchip type:MMC3B\n");
    prgram = 8192;
    //MMC6
    //output.append("\tboard type:NES-HKROM\n");
    //output.append("\t\tchip type:MMC6\n");
    //prgram = 1024;
    break;

  case   5:
    output.append("\tboard type:NES-ELROM\n");
    output.append("\t\tchip type:MMC5\n");
    prgram = 65536;
    break;

  case   7:
    output.append("\tboard type:NES-AOROM\n");
    break;

  case   9:
    output.append("\tboard type:NES-PNROM\n");
    output.append("\t\tchip type:MMC2\n");
    prgram = 8192;
    break;

  case  10:
    output.append("\tboard type:NES-FKROM\n");
    output.append("\t\tchip type:MMC4\n");
    prgram = 8192;
    break;

  case  16:
    output.append("\tboard type:BANDAI-FCG\n");
    output.append("\t\tchip type:LZ93D50\n");
    break;

  case  21:
  case  23:
  case  25:
    //VRC2
    //output.append("\tboard type:KONAMI-VRC-2\n");
    //output.append("\t\tchip type:VRC2\n");
    //VRC4
    output.append("\tboard type:KONAMI-VRC-4\n");
    output.append("\t\tchip type:VRC4\n");
    output.append("\t\t\tpinout a0=1 a1=0\n");
    prgram = 8192;
    break;

  case  24:
    output.append("\tboard type:KONAMI-VRC-6\n");
    output.append("\t\tchip type:VRC6\n");
    break;

  case  26:
    output.append("\tboard type:KONAMI-VRC-6\n");
    output.append("\t\tchip type:VRC6\n");
    prgram = 8192;
    break;

  case  34:
    output.append("\tboard type:NES-BNROM\n");
    output.append("\t\tmirror:", mirror == 0 ? "horizontal" : "vertical", "\n");
    break;

  case  66:
    output.append("\tboard type:NES-GNROM\n");
    output.append("\t\tmirror:", mirror == 0 ? "horizontal" : "vertical", "\n");
    break;

  case  69:
    output.append("\tboard type:SUNSOFT-5B\n");
    output.append("\t\tchip type:5B\n");
    prgram = 8192;
    break;

  case  73:
    output.append("\tboard type:KONAMI-VRC-3\n");
    output.append("\t\tchip type:VRC3\n");
    output.append("\t\tmirror:", mirror == 0 ? "horizontal" : "vertical", "\n");
    prgram = 8192;
    break;

  case  75:
    output.append("\tboard type:KONAMI-VRC-1\n");
    output.append("\t\tchip type:VRC1\n");
    break;

  case  85:
    output.append("\tboard type:KONAMI-VRC-7\n");
    output.append("\t\tchip type:VRC7\n");
    prgram = 8192;
    break;
  }

  output.append("\t\tprg rom=", prgrom, " ram=", prgram, "\n");
  output.append("\t\tchr rom=", chrrom, " ram=", chrram, "\n");

  print(output, "\n");

  return output;
}
