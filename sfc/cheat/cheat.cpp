#include <sfc/sfc.hpp>

#define CHEAT_CPP
namespace SuperFamicom {

Cheat cheat;

auto Cheat::reset() -> void {
  codes.reset();
}

auto Cheat::append(unsigned addr, unsigned data) -> void {
  codes.append({addr, Unused, data});
}

auto Cheat::append(unsigned addr, unsigned comp, unsigned data) -> void {
  codes.append({addr, comp, data});
}

auto Cheat::find(unsigned addr, unsigned comp) -> maybe<unsigned> {
  //WRAM mirroring: $00-3f,80-bf:0000-1fff -> $7e:0000-1fff
  if((addr & 0x40e000) == 0x000000) addr = 0x7e0000 | (addr & 0x1fff);

  for(auto& code : codes) {
    if(code.addr == addr && (code.comp == Unused || code.comp == comp)) {
      return code.data;
    }
  }

  return nothing;
}

}
