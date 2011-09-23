#include <nes/nes.hpp>

namespace NES {

Interface *interface = 0;

void Interface::videoRefresh(const uint16_t *data) {
}

void Interface::audioSample(const int16_t sample) {
}

int16_t Interface::inputPoll(bool port, unsigned device, unsigned id) {
  return 0;
}

void Interface::initialize(Interface *derived_interface) {
  interface = derived_interface;
  system.init();
}

void Interface::connect(bool port, Input::Device device) {
  input.connect(port, device);
}

bool Interface::cartridgeLoaded() {
  return cartridge.loaded();
}

void Interface::loadCartridge(const string &xml, const uint8_t *data, unsigned size) {
  cartridge.load(xml, data, size);
  system.power();
}

void Interface::unloadCartridge() {
  cartridge.unload();
}

unsigned Interface::memorySize(Memory memory) {
  if(memory == Memory::RAM) return cartridge.ram_size();
  return 0u;
}

uint8_t* Interface::memoryData(Memory memory) {
  if(memory == Memory::RAM) return cartridge.ram_data();
  return 0u;
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
      unsigned addr, data, comp;
      if(Cheat::decode(part, addr, data, comp)) {
        cheat.append({ addr, data, comp });
      }
    }
  }
  cheat.synchronize();
}

void Interface::message(const string &text) {
  print(text, "\n");
}

}
