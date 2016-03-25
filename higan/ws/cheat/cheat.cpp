#include <ws/ws.hpp>

namespace WonderSwan {

Cheat cheat;

auto Cheat::reset() -> void {
  codes.reset();
}

auto Cheat::append(uint addr, uint data) -> void {
  codes.append({addr, Unused, data});
}

auto Cheat::append(uint addr, uint comp, uint data) -> void {
  codes.append({addr, comp, data});
}

auto Cheat::find(uint addr, uint comp) -> maybe<uint> {
  for(auto& code : codes) {
    if(code.addr == addr && (code.comp == Unused || code.comp == comp)) {
      return code.data;
    }
  }
  return nothing;
}

}
