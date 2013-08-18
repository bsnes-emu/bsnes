#include <sfc/sfc.hpp>

#define SUFAMITURBO_CPP
namespace SuperFamicom {

#include "serialization.cpp"
SufamiTurboCartridge sufamiturboA;
SufamiTurboCartridge sufamiturboB;

void SufamiTurboCartridge::load() {
}

void SufamiTurboCartridge::unload() {
  rom.reset();
  ram.reset();
}

}
