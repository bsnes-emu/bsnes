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

  markup.append("cartridge\n");
  markup.append("  rom name=program.rom size=0x", hex(size), " type=FlashROM\n");
}

}

#endif
