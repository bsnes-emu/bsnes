namespace Heuristics {

struct BSMemory {
  BSMemory(const uint8_t* data, uint size);
  explicit operator bool() const;

  auto manifest() const -> string;

private:
  const uint8_t* data = nullptr;
  uint size = 0;
};

BSMemory::BSMemory(const uint8_t* data, uint size) : data(data), size(size) {
}

BSMemory::operator bool() const {
  return size == 0x100000;
}

auto BSMemory::manifest() const -> string {
  if(!operator bool()) return "";

  string output;
  output.append("game\n");
  output.append("  sha256: ", Hash::SHA256(data, size).digest(), "\n");
  output.append("  memory\n");
  output.append("    type: NAND\n");
  output.append("    size: 0x", hex(size), "\n");
  output.append("    name: program.rom\n");
  return output;
}

}
