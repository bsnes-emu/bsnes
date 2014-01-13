#include <fc/fc.hpp>

namespace Famicom {

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
  for(auto& code : codes) {
    if(code.addr == addr && (code.comp == Unused || code.comp == comp)) {
      return {true, code.data};
    }
  }
  return false;
}

}
