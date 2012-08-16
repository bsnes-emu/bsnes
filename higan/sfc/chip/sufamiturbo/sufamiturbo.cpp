#include <sfc/sfc.hpp>

#define SUFAMITURBO_CPP
namespace SuperFamicom {

#include "serialization.cpp"
SufamiTurbo sufamiturbo;

void SufamiTurbo::load() {
}

void SufamiTurbo::unload() {
  slotA.rom.reset();
  slotA.ram.reset();
  slotB.rom.reset();
  slotB.ram.reset();
}

}
