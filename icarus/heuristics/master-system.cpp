namespace Heuristics {

struct MasterSystem {
  MasterSystem(vector<uint8_t>& data, string location);
  explicit operator bool() const;
  auto manifest() const -> string;

private:
  vector<uint8_t>& data;
  string location;
};

MasterSystem::MasterSystem(vector<uint8_t>& data, string location) : data(data), location(location) {
}

MasterSystem::operator bool() const {
  return (bool)data;
}

auto MasterSystem::manifest() const -> string {
  string output;
  output.append("game\n");
  output.append("  sha256: ", Hash::SHA256(data).digest(), "\n");
  output.append("  label:  ", Location::prefix(location), "\n");
  output.append("  name:   ", Location::prefix(location), "\n");
  output.append("  board\n");
  output.append(Memory{}.type("ROM").size(data.size()).category("Program").text());
  output.append(Memory{}.type("RAM").size(0x8000).category("Save").battery().text());
  return output;
}

}
