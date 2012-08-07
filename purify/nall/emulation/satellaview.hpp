#ifndef NALL_EMULATION_SATELLAVIEW_HPP
#define NALL_EMULATION_SATELLAVIEW_HPP

#include <nall/sha256.hpp>
#include <nall/string.hpp>

namespace nall {

struct SatellaviewCartridge {
  string markup;
  inline SatellaviewCartridge(const uint8_t *data, unsigned size);
};

SatellaviewCartridge::SatellaviewCartridge(const uint8_t *data, unsigned size) {
  markup = "";

  markup.append("<?xml version='1.0' encoding='UTF-8'?>\n");
  markup.append("<cartridge sha256='", sha256(data, size) ,"'>\n");
  markup.append("  <rom name='program.rom' size='0x", hex(size), "'/>\n");
  markup.append("</cartridge>\n");
  markup.transform("'", "\"");
}

}

#endif
