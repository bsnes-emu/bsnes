#include <sfc/sfc.hpp>

#define CHEAT_CPP
namespace SuperFamicom {

Cheat cheat;

void Cheat::reset() {
  codes.reset();
}

void Cheat::append(unsigned addr, unsigned data) {
  codes.append({addr, Unused, data});
}

void Cheat::append(unsigned addr, unsigned comp, unsigned data) {
  codes.append({addr, comp, data});
}

optional<unsigned> Cheat::find(unsigned addr, unsigned comp) {
  //WRAM mirroring: $00-3f,80-bf:0000-1fff -> $7e:0000-1fff
  if((addr & 0x40e000) == 0x000000) addr = 0x7e0000 | (addr & 0x1fff);

  for(auto& code : codes) {
    if(code.addr == addr && (code.comp == Unused || code.comp == comp)) {
      return {true, code.data};
    }
  }
  return false;
}

}
