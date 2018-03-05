namespace Heuristics {

struct SuperGrafx {
  SuperGrafx(vector<uint8_t>& data, string location);
  explicit operator bool() const;
  auto manifest() const -> string;

private:
  vector<uint8_t>& data;
  string location;
};

SuperGrafx::operator bool() const {
  return (bool)data;
}

SuperGrafx::SuperGrafx(vector<uint8_t>& data, string location) : data(data), location(location) {
}

auto SuperGrafx::manifest() const -> string {
  string output;
  output.append("game\n");
  output.append("  sha256: ", Hash::SHA256(data).digest(), "\n");
  output.append("  label: ", Location::prefix(location), "\n");
  output.append("  name: ", Location::prefix(location), "\n");
  output.append("  board\n");
  output.append(Memory{}.type("ROM").size(data.size()).category("Program").text());
  return output;
}

}
