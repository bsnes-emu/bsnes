namespace Heuristics {

struct SufamiTurbo {
  SufamiTurbo(const uint8_t* data, uint size);
  explicit operator bool() const;

  auto manifest() const -> string;

private:
  const uint8_t* data = nullptr;
  uint size = 0;
};

SufamiTurbo::SufamiTurbo(const uint8_t* data, uint size) : data(data), size(size) {
}

SufamiTurbo::operator bool() const {
  return size >= 0x20000;
}

auto SufamiTurbo::manifest() const -> string {
  if(!operator bool()) return "";

  uint romSize = data[0x36] * 0x20000;  //128KB
  uint ramSize = data[0x37] * 0x800;  //2KB
  bool linkable = data[0x35];  //TODO: unconfirmed

  string output;
  output.append("game\n");
  if(linkable) {
    output.append("  linkable\n");
  }
  if(romSize) {
    output.append("  memory\n");
    output.append("    type: ROM\n");
    output.append("    size: 0x", hex(romSize), "\n");
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
