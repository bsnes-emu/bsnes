#ifndef NALL_EMULATION_SUFAMI_TURBO_HPP
#define NALL_EMULATION_SUFAMI_TURBO_HPP

#include <nall/sha256.hpp>
#include <nall/string.hpp>

namespace nall {

struct SufamiTurboCartridge {
  string markup;
  inline SufamiTurboCartridge(const uint8_t *data, unsigned size);
};

SufamiTurboCartridge::SufamiTurboCartridge(const uint8_t *data, unsigned size) {
  markup = "";

  if(size < 0x20000) return;  //too small to be a valid game?
  if(memcmp(data, "BANDAI SFC-ADX", 14)) return;  //missing required header?
  unsigned romsize = data[0x36] * 0x20000;  //128KB
  unsigned ramsize = data[0x37] *   0x800;    //2KB
  bool linkable = data[0x35] != 0x00;  //TODO: unconfirmed

  markup.append("<?xml version='1.0' encoding='UTF-8'?>\n");
  markup.append("<cartridge linkable='", linkable, "' sha256='", sha256(data, size) ,"'>\n");
  markup.append("  <rom name='program.rom' size='0x", hex(romsize), "'/>\n");
  markup.append("  <ram name='save.ram' size='0x", hex(ramsize), "'/>\n");
  markup.append("</cartridge>\n");
  markup.transform("'", "\"");
}

}

#endif
