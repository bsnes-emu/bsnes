namespace Heuristics {

struct BSMemory {
  BSMemory(vector<uint8_t>& data, string location);
  explicit operator bool() const;

  auto manifest() const -> string;

private:
  vector<uint8_t>& data;
  string location;
};

BSMemory::BSMemory(vector<uint8_t>& data, string location) : data(data), location(location) {
}

BSMemory::operator bool() const {
  return data.size() >= 0x8000;
}

auto BSMemory::manifest() const -> string {
  if(!operator bool()) return "";

  string output;
  output.append("game\n");
  output.append("  sha256: ", Hash::SHA256(data).digest(), "\n");
  output.append("  name:   ", Location::prefix(location), "\n");
  output.append("  label:  ", Location::prefix(location), "\n");
  output.append("  memory\n");
  output.append("    type: NAND\n");
  output.append("    size: 0x", hex(data.size()), "\n");
  output.append("    name: program.rom\n");
  return output;
}

}
