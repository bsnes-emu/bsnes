#include <sfc/sfc.hpp>

namespace SuperFamicom {

#include "serialization.cpp"
SufamiTurboCartridge sufamiturboA;
SufamiTurboCartridge sufamiturboB;

auto SufamiTurboCartridge::load() -> void {
}

auto SufamiTurboCartridge::unload() -> void {
  rom.reset();
  ram.reset();
}

}
