#include <sfc/sfc.hpp>

#define SUFAMITURBO_CPP
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
