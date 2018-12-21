namespace Heuristics {

struct SG1000 {
  SG1000(vector<uint8_t>& data, string location);
  explicit operator bool() const;
  auto manifest() const -> string;

private:
  vector<uint8_t>& data;
  string location;
};

SG1000::SG1000(vector<uint8_t>& data, string location) : data(data), location(location) {
}

SG1000::operator bool() const {
  return (bool)data;
}

auto SG1000::manifest() const -> string {
  string output;
  output.append("game\n");
  output.append("  sha256: ", Hash::SHA256(data).digest(), "\n");
  output.append("  label:  ", Location::prefix(location), "\n");
  output.append("  name:   ", Location::prefix(location), "\n");
  output.append("  board\n");
  output.append(Memory{}.type("ROM").size(data.size()).content("Program").text());
  output.append(Memory{}.type("RAM").size(0x8000).content("Save").text());
  return output;
}

}
