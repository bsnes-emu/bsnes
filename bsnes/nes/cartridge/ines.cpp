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

  case 1:
    output.append("  board type=NES-SXROM\n");
    break;

  case 2:
    output.append("  board type=NES-UOROM\n");
    output.append("    mirror=", mirror == 0 ? "horizontal" : "vertical", "\n");
    break;

  case 3:
    output.append("  board type=NES-CNROM\n");
    output.append("    mirror=", mirror == 0 ? "horizontal" : "vertical", "\n");
    break;

  case 7:
    output.append("  board type=NES-AOROM\n");
    break;
  }

  output.append("    prg rom=", prgrom, " ram=", prgram, "\n");
  output.append("    chr rom=", chrrom, " ram=", chrram, "\n");

  print(output, "\n");

  return output;
}

/*
  switch(mapperNumber) {
//default : mapper = &Mapper::none; break;
//case   1: mapper = &Mapper::mmc1; break;
//case   2: mapper = &Mapper::uorom; break;
//case   3: mapper = &Mapper::cnrom; break;
  case   4: mapper = &Mapper::mmc3; break;
//case   7: mapper = &Mapper::aorom; break;
  case  16: mapper = &Mapper::bandaiFCG; break;
  case  24: mapper = &Mapper::vrc6; Mapper::vrc6.abus_swap = 0; break;
  case  26: mapper = &Mapper::vrc6; Mapper::vrc6.abus_swap = 1; break;
  }
*/
