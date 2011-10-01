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
    output.append("  board type=NES-NROM-256\n");
    output.append("    mirror=", mirror == 0 ? "horizontal" : "vertical", "\n");
    break;

  case   1:
    output.append("  board type=NES-SXROM\n");
    output.append("    chip type=MMC1B2\n");
    prgram = 8192;
    break;

  case   2:
    output.append("  board type=NES-UOROM\n");
    output.append("    mirror=", mirror == 0 ? "horizontal" : "vertical", "\n");
    break;

  case   3:
    output.append("  board type=NES-CNROM\n");
    output.append("    mirror=", mirror == 0 ? "horizontal" : "vertical", "\n");
    break;

  case   4:
    output.append("  board type=NES-TLROM\n");
    output.append("    chip type=MMC3B\n");
    prgram = 8192;
    break;

  case   7:
    output.append("  board type=NES-AOROM\n");
    break;

  case  16:
    output.append("  board type=BANDAI-FCG\n");
    output.append("    chip type=LZ93D50 manufacturer=Sharp\n");
    break;

  case 24:
    output.append("  board type=KONAMI-VRC-6\n");
    output.append("    chip type=VRC6\n");
    break;

  case 26:
    output.append("  board type=KONAMI-VRC-6\n");
    output.append("    chip type=VRC6\n");
    prgram = 8192;
    break;
  }

  output.append("    prg rom=", prgrom, " ram=", prgram, "\n");
  output.append("    chr rom=", chrrom, " ram=", chrram, "\n");

  print(output, "\n");

  return output;
}
