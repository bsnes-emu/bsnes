#include <snes/snes.hpp>

namespace SNES {

Interface *interface = 0;

void Interface::videoRefresh(const uint16_t *data, bool hires, bool interlace, bool overscan) {
}

void Interface::audioSample(int16_t l_sample, int16_t r_sample) {
}

int16_t Interface::inputPoll(bool port, Input::Device device, unsigned index, unsigned id) {
  return 0;
}

void Interface::initialize(Interface *derived_interface) {
  interface = derived_interface;
  system.init();
}

bool Interface::cartridgeLoaded() {
  return cartridge.loaded();
}

void Interface::loadCartridge(const string &xml, const uint8_t *data, unsigned size) {
  cartridge.rom.copy(data, size);
  cartridge.load(Cartridge::Mode::Normal, { xml });
  system.power();
}

void Interface::unloadCartridge() {
  cartridge.unload();
}

void Interface::power() {
  system.power();
}

void Interface::reset() {
  system.reset();
}

void Interface::run() {
  system.run();
}

serializer Interface::serialize() {
  system.runtosave();
  return system.serialize();
}

bool Interface::unserialize(serializer &s) {
  return system.unserialize(s);
}

void Interface::setCheats(const lstring &list) {
  cheat.reset();
  foreach(code, list) {
    lstring codelist;
    codelist.split("+", code);
    foreach(part, codelist) {
      unsigned addr, data;
      Cheat::Type type;
      if(Cheat::decode(part, addr, data, type)) {
        cheat.append({ addr, data });
      }
    }
  }
  cheat.synchronize();
}

void Interface::message(const string &text) {
  print(text, "\n");
}

}
