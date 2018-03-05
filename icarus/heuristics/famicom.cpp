namespace Heuristics {

struct Famicom {
  Famicom(vector<uint8_t>& data, string location);
  explicit operator bool() const;
  auto manifest() const -> string;

private:
  vector<uint8_t>& data;
  string location;
};

Famicom::Famicom(vector<uint8_t>& data, string location) : data(data), location(location) {
}

Famicom::operator bool() const {
  if(data.size() < 16) return false;
  if(data[0] != 'N') return false;
  if(data[1] != 'E') return false;
  if(data[2] != 'S') return false;
  if(data[3] !=  26) return false;
  return true;
}

auto Famicom::manifest() const -> string {
  if(!operator bool()) return {};

  uint mapper = ((data[7] >> 4) << 4) | (data[6] >> 4);
  uint mirror = ((data[6] & 0x08) >> 2) | (data[6] & 0x01);
  uint prgrom = data[4] * 0x4000;
  uint chrrom = data[5] * 0x2000;
  uint prgram = 0u;
  uint chrram = chrrom == 0u ? 8192u : 0u;

  string output;
  output.append("game\n");
  output.append("  sha256: ", Hash::SHA256(&data[16], data.size() - 16).digest(), "\n");
  output.append("  label: ", Location::prefix(location), "\n");
  output.append("  name: ", Location::prefix(location), "\n");

  switch(mapper) {
  default:
    output.append("  board: NES-NROM-256\n");
    output.append("    mirror mode=", mirror == 0 ? "horizontal" : "vertical", "\n");
    break;

  case   1:
    output.append("  board: NES-SXROM\n");
    output.append("    chip type=MMC1B2\n");
    prgram = 8192;
    break;

  case   2:
    output.append("  board: NES-UOROM\n");
    output.append("    mirror mode=", mirror == 0 ? "horizontal" : "vertical", "\n");
    break;

  case   3:
    output.append("  board: NES-CNROM\n");
    output.append("    mirror mode=", mirror == 0 ? "horizontal" : "vertical", "\n");
    break;

  case   4:
    //MMC3
    output.append("  board: NES-TLROM\n");
    output.append("    chip type=MMC3B\n");
    prgram = 8192;
    //MMC6
  //output.append("  board: NES-HKROM\n");
  //output.append("    chip type=MMC6\n");
  //prgram = 1024;
    break;

  case   5:
    output.append("  board: NES-ELROM\n");
    output.append("    chip type=MMC5\n");
    prgram = 65536;
    break;

  case   7:
    output.append("  board: NES-AOROM\n");
    break;

  case   9:
    output.append("  board: NES-PNROM\n");
    output.append("    chip type=MMC2\n");
    prgram = 8192;
    break;

  case  10:
    output.append("  board: NES-FKROM\n");
    output.append("    chip type=MMC4\n");
    prgram = 8192;
    break;

  case  16:
    output.append("  board: BANDAI-FCG\n");
    output.append("    chip type=LZ93D50\n");
    break;

  case  21:
  case  23:
  case  25:
    //VRC4
    output.append("  board: KONAMI-VRC-4\n");
    output.append("    chip type=VRC4\n");
    output.append("      pinout a0=1 a1=0\n");
    prgram = 8192;
    break;

  case  22:
    //VRC2
    output.append("  board: KONAMI-VRC-2\n");
    output.append("    chip type=VRC2\n");
    output.append("      pinout a0=0 a1=1\n");
    break;

  case  24:
    output.append("  board: KONAMI-VRC-6\n");
    output.append("    chip type=VRC6\n");
    break;

  case  26:
    output.append("  board: KONAMI-VRC-6\n");
    output.append("    chip type=VRC6\n");
    prgram = 8192;
    break;

  case  34:
    output.append("  board: NES-BNROM\n");
    output.append("    mirror mode=", mirror == 0 ? "horizontal" : "vertical", "\n");
    break;

  case  66:
    output.append("  board: NES-GNROM\n");
    output.append("    mirror mode=", mirror == 0 ? "horizontal" : "vertical", "\n");
    break;

  case  69:
    output.append("  board: SUNSOFT-5B\n");
    output.append("    chip type=5B\n");
    prgram = 8192;
    break;

  case  73:
    output.append("  board: KONAMI-VRC-3\n");
    output.append("    chip type=VRC3\n");
    output.append("    mirror mode=", mirror == 0 ? "horizontal" : "vertical", "\n");
    prgram = 8192;
    break;

  case  75:
    output.append("  board: KONAMI-VRC-1\n");
    output.append("    chip type=VRC1\n");
    break;

  case  85:
    output.append("  board: KONAMI-VRC-7\n");
    output.append("    chip type=VRC7\n");
    prgram = 8192;
    break;
  }

  if(prgrom) output.append(Memory{}.type("ROM").size(prgrom).category("Program").text());
  if(prgram) output.append(Memory{}.type("RAM").size(prgram).category("Save").battery().text());

  if(chrrom) output.append(Memory{}.type("ROM").size(chrrom).category("Character").text());
  if(chrram) output.append(Memory{}.type("RAM").size(chrram).category("Character").text());

  return output;
}

}
