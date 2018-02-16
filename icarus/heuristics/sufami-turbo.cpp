namespace Heuristics {

struct SufamiTurbo {
  SufamiTurbo(vector<uint8_t>& data, string location);
  explicit operator bool() const;

  auto manifest() const -> string;

private:
  vector<uint8_t>& data;
  string location;
};

SufamiTurbo::SufamiTurbo(vector<uint8_t>& data, string location) : data(data), location(location) {
}

SufamiTurbo::operator bool() const {
  return data.size() >= 0x20000;
}

auto SufamiTurbo::manifest() const -> string {
  if(!operator bool()) return "";

  uint romSize = data[0x36] * 0x20000;  //128KB
  uint ramSize = data[0x37] *   0x800;  //  2KB

  string output;
  output.append("game\n");
  output.append("  name:  ", Location::prefix(location), "\n");
  output.append("  label: ", Location::prefix(location), "\n");
  if(romSize) {
    output.append("  memory\n");
    output.append("    type: ROM\n");
    output.append("    size: 0x", hex(data.size()), "\n");
    output.append("    name: program.rom\n");
  }
  if(ramSize) {
    output.append("  memory\n");
    output.append("    type: NVRAM\n");
    output.append("    size: 0x", hex(ramSize), "\n");
    output.append("    name: save.ram\n");
  }
  return output;
}

}
