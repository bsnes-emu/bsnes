namespace Heuristics {

struct WonderSwan {
  WonderSwan(vector<uint8_t>& buffer, string location);
  explicit operator bool() const;
  auto manifest() const -> string;

private:
  vector<uint8_t>& data;
  string location;
};

WonderSwan::WonderSwan(vector<uint8_t>& data, string location) : data(data), location(location) {
}

WonderSwan::operator bool() const {
  return data.size() >= 0x10000;
}

auto WonderSwan::manifest() const -> string {
  if(!operator bool()) return {};

  auto metadata = &data[data.size() - 16];

  bool color = metadata[7];

  string ramType;
  uint ramSize = 0;
  switch(metadata[11]) {
  case 0x01: ramType = "RAM";    ramSize =    8 * 1024; break;
  case 0x02: ramType = "RAM";    ramSize =   32 * 1024; break;
  case 0x03: ramType = "RAM";    ramSize =  128 * 1024; break;
  case 0x04: ramType = "RAM";    ramSize =  256 * 1024; break;
  case 0x05: ramType = "RAM";    ramSize =  512 * 1024; break;
  case 0x10: ramType = "EEPROM"; ramSize =  128;        break;
  case 0x20: ramType = "EEPROM"; ramSize = 2048;        break;
  case 0x50: ramType = "EEPROM"; ramSize = 1024;        break;
  }

  bool orientation = metadata[12] & 1;  //0 = horizontal; 1 = vertical
  bool hasRTC = metadata[13] & 1;

  string output;
  output.append("game\n");
  output.append("  sha256:      ", Hash::SHA256(data).digest(), "\n");
  output.append("  label:       ", Location::prefix(location), "\n");
  output.append("  name:        ", Location::prefix(location), "\n");
  output.append("  orientation: ", !orientation ? "horizontal" : "vertical", "\n");
  output.append("  board\n");
  output.append(Memory{}.type("ROM").size(data.size()).content("Program").text());
if(ramType && ramSize)
  output.append(Memory{}.type(ramType).size(ramSize).content("Save").text());
if(hasRTC)
  output.append(Memory{}.type("RTC").size(0x10).content("Time").text());
  return output;
}

}
