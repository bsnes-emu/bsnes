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

//print("iNES mapper: ", mapper, "\n");

  output.append("<?xml version='1.0' encoding='UTF-8'?>\n");
  output.append("<cartridge>\n");

  switch(mapper) {
  default:
    output.append("  <board type='NES-NROM-256'>\n");
    output.append("    <mirror mode='", mirror == 0 ? "horizontal" : "vertical", "'/>\n");
    break;

  case   1:
    output.append("  <board type='NES-SXROM'>\n");
    output.append("    <chip type='MMC1B2'/>\n");
    prgram = 8192;
    break;

  case   2:
    output.append("  <board type='NES-UOROM'>\n");
    output.append("    <mirror mode='", mirror == 0 ? "horizontal" : "vertical", "'/>\n");
    break;

  case   3:
    output.append("  <board type='NES-CNROM'>\n");
    output.append("    <mirror mode='", mirror == 0 ? "horizontal" : "vertical", "'/>\n");
    break;

  case   4:
    //MMC3
    output.append("  <board type='NES-TLROM'>\n");
    output.append("    <chip type='MMC3B'/>\n");
    prgram = 8192;
    //MMC6
    //output.append("  <board type='NES-HKROM'>\n");
    //output.append("    <chip type='MMC6'/>\n");
    //prgram = 1024;
    break;

  case   5:
    output.append("  <board type='NES-ELROM'>\n");
    output.append("    <chip type='MMC5'/>\n");
    prgram = 65536;
    break;

  case   7:
    output.append(" <board type='NES-AOROM'>\n");
    break;

  case   9:
    output.append("  <board type='NES-PNROM'>\n");
    output.append("    <chip type='MMC2'/>\n");
    prgram = 8192;
    break;

  case  10:
    output.append("  <board type='NES-FKROM'>\n");
    output.append("    <chip type='MMC4'/>\n");
    prgram = 8192;
    break;

  case  16:
    output.append("  <board type='BANDAI-FCG'>\n");
    output.append("    <chip type='LZ93D50'/>\n");
    break;

  case  21:
  case  23:
  case  25:
    //VRC4
    output.append("  <board type='KONAMI-VRC-4'>\n");
    output.append("    <chip type='VRC4'>\n");
    output.append("      <pinout a0='1' a1='0'/>\n");
    output.append("    </chip>\n");
    prgram = 8192;
    break;

  case  22:
    //VRC2
    output.append("  <board type='KONAMI-VRC-2'>\n");
    output.append("    <chip type='VRC2'>\n");
    output.append("      <pinout a0='0' a1='1'/>\n");
    output.append("    </chip>\n");
    break;

  case  24:
    output.append("  <board type='KONAMI-VRC-6'>\n");
    output.append("    <chip type='VRC6'/>\n");
    break;

  case  26:
    output.append("  <board type='KONAMI-VRC-6'>\n");
    output.append("    <chip type='VRC6'/>\n");
    prgram = 8192;
    break;

  case  34:
    output.append("  <board type='NES-BNROM'>\n");
    output.append("    <mirror type='", mirror == 0 ? "horizontal" : "vertical", "'/>\n");
    break;

  case  66:
    output.append("  <board type='NES-GNROM'>\n");
    output.append("    <mirror type='", mirror == 0 ? "horizontal" : "vertical", "'/>\n");
    break;

  case  69:
    output.append("  <board type='SUNSOFT-5B'>\n");
    output.append("    <chip type='5B'/>\n");
    prgram = 8192;
    break;

  case  73:
    output.append("  <board type='KONAMI-VRC-3'>\n");
    output.append("    <chip type='VRC3'/>\n");
    output.append("    <mirror type='", mirror == 0 ? "horizontal" : "vertical", "'/>\n");
    prgram = 8192;
    break;

  case  75:
    output.append("  <board type='KONAMI-VRC-1'>\n");
    output.append("    <chip type='VRC1'/>\n");
    break;

  case  85:
    output.append("  <board type='KONAMI-VRC-7'>\n");
    output.append("    <chip type='VRC7'/>\n");
    prgram = 8192;
    break;
  }

  output.append(
    "    <prg rom='0x", hex(prgrom), "' ram='0x", hex(prgram), "'/>\n"
    "    <chr rom='0x", hex(chrrom), "' ram='0x", hex(chrram), "'/>\n"
    "  </board>\n"
    "</cartridge>\n"
  );

  print(output, "\n");

  return output;
}
