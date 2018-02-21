namespace Heuristics {

struct SufamiTurbo : Heuristics {
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
  if(!operator bool()) return {};

  uint romSize = data[0x36] * 0x20000;  //128KB
  uint ramSize = data[0x37] *   0x800;  //  2KB

  string output;
  output.append("game\n");
  output.append("  sha256: ", Hash::SHA256(data).digest(), "\n");
  output.append("  name:   ", Location::prefix(location), "\n");
  output.append("  label:  ", Location::prefix(location), "\n");
  output.append(memory("ROM", data.size(), "program.rom"));
  if(ramSize) output.append(memory("NVRAM", ramSize, "save.ram"));
  return output;
}

}
