#include <gameboy/gameboy.hpp>

namespace GameBoy {

Cheat cheat;

bool Cheat::decode(const string &code, unsigned &addr, unsigned &data, unsigned &comp) {
  static bool initialize = false;
  static uint8 asciiMap[256];

  if(initialize == false) {
    initialize = true;
    foreach(n, asciiMap) n = ~0;
    asciiMap['0'] =  0; asciiMap['1'] =  1; asciiMap['2'] =  2; asciiMap['3'] =  3;
    asciiMap['4'] =  4; asciiMap['5'] =  5; asciiMap['6'] =  6; asciiMap['7'] =  7;
    asciiMap['8'] =  8; asciiMap['9'] =  9; asciiMap['A'] = 10; asciiMap['B'] = 11;
    asciiMap['C'] = 12; asciiMap['D'] = 13; asciiMap['E'] = 14; asciiMap['F'] = 15;
  }

  unsigned length = code.length(), bits = 0;
  for(unsigned n = 0; n < length; n++) if(asciiMap[code[n]] > 15 && code[n] != '-') return false;

  if(code.wildcard("???" "-" "???")) {
    string text = string(code).replace("-", "");
    for(unsigned n = 0; n < 6; n++) bits |= asciiMap[text[n]] << (20 - n * 4);

    addr = (bits >> 0) & 0xffff;
    data = (bits >> 16) & 0xff;
    comp = ~0;

    addr = (((addr >> 4) | (addr << 12)) & 0xffff) ^ 0xf000;

    return true;
  }

  if(code.wildcard("???" "-" "???" "-" "???")) {
    string text = string(code).replace("-", "");
    for(unsigned n = 0; n < 8; n++) bits |= asciiMap[text[n == 7 ? 8 : n]] << (28 - n * 4);

    addr = (bits >> 8) & 0xffff;
    data = (bits >> 24) & 0xff;
    comp = (bits >> 0) & 0xff;

    addr = (((addr >> 4) | (addr << 12)) & 0xffff) ^ 0xf000;
    comp = (((comp >> 2) | (comp << 6)) & 0xff) ^ 0xba;

    return true;
  }

  return false;
}

void Cheat::synchronize() {
  foreach(n, override) n = false;

  for(unsigned n = 0; n < size(); n++) {
    const CheatCode &code = operator[](n);

    for(unsigned n = 0; n < code.addr.size(); n++) {
      override[code.addr[n]] = true;
    }
  }
}

bool CheatCode::operator=(const string &s) {
  addr.reset(), data.reset(), comp.reset();

  lstring list;
  list.split("+", s);

  for(unsigned n = 0; n < list.size(); n++) {
    unsigned codeaddr, codedata, codecomp;
    if(Cheat::decode(list[n], codeaddr, codedata, codecomp) == false) {
      addr.reset(), data.reset(), comp.reset();
      return false;
    }
    addr.append(codeaddr), data.append(codedata), comp.append(codecomp);
  }

  return true;
}

CheatCode::CheatCode(const string &s) {
  operator=(s);
}

CheatCode::CheatCode() {
  enable = false;
}

}
