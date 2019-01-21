namespace Heuristics {

struct NeoGeoPocket {
  NeoGeoPocket(vector<uint8_t>& data, string location);
  explicit operator bool() const;
  auto manifest() const -> string;
  auto title() const -> string;

private:
  vector<uint8_t>& data;
  string location;
};

NeoGeoPocket::NeoGeoPocket(vector<uint8_t>& data, string location) : data(data), location(location) {
}

NeoGeoPocket::operator bool() const {
  switch(data.size()) {
  case 0x080000: return true;  // 4mbit
  case 0x100000: return true;  // 8mbit
  case 0x200000: return true;  //16mbit
  case 0x280000: return true;  //20mbit
  case 0x300000: return true;  //24mbit
  case 0x400000: return true;  //32mbit
  }
  return false;
}

auto NeoGeoPocket::manifest() const -> string {
  if(!operator bool()) return {};

  string output;
  output.append("game\n");
  output.append("  sha256: ", Hash::SHA256(data).digest(), "\n");
  output.append("  label:  ", Location::prefix(location), "\n");
  output.append("  name:   ", Location::prefix(location), "\n");
  output.append("  title:  ", title(), "\n");
  output.append("  board\n");
  output.append(Memory{}.type("ROM").size(data.size()).content("Program").text());
  return output;
}

auto NeoGeoPocket::title() const -> string {
  if(!operator bool()) return {};

  string title;
  title.size(12);
  for(uint index : range(12)) {
    char letter = data[0x24 + index];
    if(letter >= 0x20 && letter <= 0x7e) title.get()[index] = letter;
  }
  return title.strip();
}

}
