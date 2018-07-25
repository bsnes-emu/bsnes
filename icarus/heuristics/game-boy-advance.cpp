namespace Heuristics {

struct GameBoyAdvance {
  GameBoyAdvance(vector<uint8_t>& buffer, string location);
  explicit operator bool() const;
  auto manifest() const -> string;

private:
  vector<uint8_t>& data;
  string location;
};

GameBoyAdvance::GameBoyAdvance(vector<uint8_t>& data, string location) : data(data), location(location) {
}

GameBoyAdvance::operator bool() const {
  return (bool)data;
}

auto GameBoyAdvance::manifest() const -> string {
  if(!operator bool()) return {};

  vector<string> identifiers = {
    "SRAM_V",
    "SRAM_F_V",
    "EEPROM_V",
    "FLASH_V",
    "FLASH512_V",
    "FLASH1M_V",
  };

  vector<string> list;
  for(auto& identifier : identifiers) {
    for(int n : range(data.size() - 16)) {
      if(!memory::compare(&data[n], identifier.data(), identifier.size())) {
        auto p = (const char*)&data[n + identifier.size()];
        if(p[0] >= '0' && p[0] <= '9'
        && p[1] >= '0' && p[1] <= '9'
        && p[2] >= '0' && p[2] <= '9'
        ) {
          char text[16];
          memory::copy(text, &data[n], identifier.size() + 3);
          text[identifier.size() + 3] = 0;
          if(!list.find(text)) list.append(text);
        }
      }
    }
  }

  string output;
  output.append("game\n");
  output.append("  sha256: ", Hash::SHA256(data).digest(), "\n");
  output.append("  label:  ", Location::prefix(location), "\n");
  output.append("  name:   ", Location::prefix(location), "\n");
  output.append("  board\n");
  output.append(Memory{}.type("ROM").size(data.size()).content("Program").text());
  if(!list);
  else if(list.left().beginsWith("SRAM_V"    )) output.append(Memory{}.type("RAM"   ).size( 0x8000).content("Save").text());
  else if(list.left().beginsWith("SRAM_F_V"  )) output.append(Memory{}.type("RAM"   ).size( 0x8000).content("Save").text());
  else if(list.left().beginsWith("EEPROM_V"  )) output.append(Memory{}.type("EEPROM").size(    0x0).content("Save").text());
  else if(list.left().beginsWith("FLASH_V"   )) output.append(Memory{}.type("Flash" ).size(0x10000).content("Save").manufacturer("Macronix").text());
  else if(list.left().beginsWith("FLASH512_V")) output.append(Memory{}.type("Flash" ).size(0x10000).content("Save").manufacturer("Macronix").text());
  else if(list.left().beginsWith("FLASH1M_V" )) output.append(Memory{}.type("Flash" ).size(0x20000).content("Save").manufacturer("Macronix").text());
  return output;
}

}
