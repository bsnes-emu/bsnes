#include <nes/nes.hpp>

namespace NES {

Cheat cheat;

bool Cheat::decode(const string &code, unsigned &addr, unsigned &data, unsigned &comp) {
  static bool initialize = false;
  static uint8 asciiMap[256];

  if(initialize == false) {
    initialize = true;
    foreach(n, asciiMap) n = ~0;
    asciiMap['A'] =  0; asciiMap['P'] =  1; asciiMap['Z'] =  2; asciiMap['L'] =  3;
    asciiMap['G'] =  4; asciiMap['I'] =  5; asciiMap['T'] =  6; asciiMap['Y'] =  7;
    asciiMap['E'] =  8; asciiMap['O'] =  9; asciiMap['X'] = 10; asciiMap['U'] = 11;
    asciiMap['K'] = 12; asciiMap['S'] = 13; asciiMap['V'] = 14; asciiMap['N'] = 15;
  }

  unsigned length = code.length(), bits = 0;
  for(unsigned n = 0; n < length; n++) if(asciiMap[code[n]] > 15) return false;

  if(code.length() == 6) {
    for(unsigned n = 0; n < 6; n++) bits |= asciiMap[code[n]] << (20 - n * 4);
    unsigned addrTable[] = { 10, 9, 8, 7, 2, 1, 0, 19, 14, 13, 12, 11, 6, 5, 4 };
    unsigned dataTable[] = { 23, 18, 17, 16, 3, 22, 21, 20 };

    addr = 0x8000, data = 0x00, comp = ~0;
    for(unsigned n = 0; n < 15; n++) addr |= bits & (1 << addrTable[n]) ? 0x4000 >> n : 0;
    for(unsigned n = 0; n <  8; n++) data |= bits & (1 << dataTable[n]) ?   0x80 >> n : 0;
    return true;
  }

  if(code.length() == 8) {
    for(unsigned n = 0; n < 8; n++) bits |= asciiMap[code[n]] << (28 - n * 4);
    unsigned addrTable[] = { 18, 17, 16, 15, 10, 9, 8, 27, 22, 21, 20, 19, 14, 13, 12 };
    unsigned dataTable[] = { 31, 26, 25, 24, 3, 30, 29, 28 };
    unsigned compTable[] = { 7, 2, 1, 0, 11, 6, 5,4 };

    addr = 0x8000, data = 0x00, comp = 0x00;
    for(unsigned n = 0; n < 15; n++) addr |= bits & (1 << addrTable[n]) ? 0x4000 >> n : 0;
    for(unsigned n = 0; n <  8; n++) data |= bits & (1 << dataTable[n]) ?   0x80 >> n : 0;
    for(unsigned n = 0; n <  8; n++) comp |= bits & (1 << compTable[n]) ?   0x80 >> n : 0;
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
