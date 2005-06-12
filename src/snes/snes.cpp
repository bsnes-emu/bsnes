#include "../base.h"

void SNES::run() {
  clock->run();
}

void SNES::power() {
  clock->power();
  cpu->power();
  ppu->power();
  mem_bus->power();

int i;
  mem_bus->flush_mmio_mappers();
  for(i=0x2100;i<=0x213f;i++)mem_bus->set_mmio_mapper(i, ppu->mmio);
  for(i=0x2140;i<=0x2143;i++)mem_bus->set_mmio_mapper(i, ppu->mmio);
  for(i=0x2180;i<=0x2183;i++)mem_bus->set_mmio_mapper(i, cpu->mmio);
  mem_bus->set_mmio_mapper(0x21c2, cpu->mmio);
  mem_bus->set_mmio_mapper(0x21c3, cpu->mmio);
  mem_bus->set_mmio_mapper(0x4016, cpu->mmio);
  mem_bus->set_mmio_mapper(0x4017, cpu->mmio);
  for(i=0x4200;i<=0x421f;i++)mem_bus->set_mmio_mapper(i, cpu->mmio);
  for(i=0x4300;i<=0x437f;i++)mem_bus->set_mmio_mapper(i, cpu->mmio);
}

void SNES::reset() {
  clock->reset();
  cpu->reset();
  ppu->reset();
  mem_bus->reset();
}

/***************************
 *** Debugging functions ***
 ***************************/
void SNES::notify(uint32 message, uint32 param1, uint32 param2) {}

void SNES::debugger_enable() {
  is_debugger_enabled = true;
}

void SNES::debugger_disable() {
  is_debugger_enabled = false;
}

bool SNES::debugger_enabled() {
  return is_debugger_enabled;
}

SNES::SNES() {
  is_debugger_enabled = true;
}
