#pragma once

namespace Emulator {

struct Cheat {
  struct Code {
    uint addr;
    uint data;
    maybe<uint> comp;
  };

  explicit operator bool() const {
    return codes.size() > 0;
  }

  auto reset() -> void {
    codes.reset();
  }

  auto append(uint addr, uint data, maybe<uint> comp = nothing) -> void {
    codes.append({addr, data, comp});
  }

  auto assign(const string_vector& list) -> void {
    reset();
    for(auto& entry : list) {
      for(auto code : entry.split("+")) {
        auto part = code.split("/");
        if(part.size() == 2) append(part[0].hex(), part[1].hex());
        if(part.size() == 3) append(part[0].hex(), part[2].hex(), part[1].hex());
      }
    }
  }

  auto find(uint addr, uint comp) -> maybe<uint> {
    for(auto& code : codes) {
      if(code.addr == addr && (!code.comp || code.comp() == comp)) {
        return code.data;
      }
    }
    return nothing;
  }

private:
  vector<Code> codes;
};

}
