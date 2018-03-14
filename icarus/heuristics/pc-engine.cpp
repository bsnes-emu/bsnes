namespace Heuristics {

struct PCEngine {
  PCEngine(vector<uint8_t>& data, string location);
  explicit operator bool() const;
  auto manifest() const -> string;

private:
  vector<uint8_t>& data;
  string location;
};

PCEngine::PCEngine(vector<uint8_t>& data, string location) : data(data), location(location) {
  if((data.size() & 0x1fff) == 512) {
    //remove header if present
    memory::move(&data[0], &data[512], data.size() - 512);
    data.resize(data.size() - 512);
  }
}

PCEngine::operator bool() const {
  return (bool)data;
}

auto PCEngine::manifest() const -> string {
  string output;
  output.append("game\n");
  output.append("  sha256: ", Hash::SHA256(data).digest(), "\n");
  output.append("  label:  ", Location::prefix(location), "\n");
  output.append("  name:   ", Location::prefix(location), "\n");
  output.append("  board\n");
  output.append(Memory{}.type("ROM").size(data.size()).category("Program").text());
  return output;
}

}
