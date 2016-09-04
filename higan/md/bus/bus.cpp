#include <md/md.hpp>

namespace MegaDrive {

BusCPU busCPU;
BusAPU busAPU;

auto BusCPU::readByte(uint24 addr) -> uint16 {
  if(addr < 0x400000) return cartridge.read(addr & ~1).byte(!addr.bit(0));
  if(addr < 0xa00000) return 0x0000;
  if(addr < 0xa10000) return 0x0000;
  if(addr < 0xa10020) return readIO(addr);
  if(addr < 0xc00000) return 0x0000;
  if(addr < 0xe00000) return vdp.read(addr & ~1).byte(!addr.bit(0));
  return ram[addr & 0xffff];
}

auto BusCPU::readWord(uint24 addr) -> uint16 {
  if(addr < 0x400000) return cartridge.read(addr);
  if(addr < 0xa00000) return 0x0000;
  if(addr < 0xa10000) return 0x0000;
  if(addr < 0xa10020) return readIO(addr);
  if(addr < 0xc00000) return 0x0000;
  if(addr < 0xe00000) return vdp.read(addr);
  uint16 data = ram[addr + 0 & 0xffff] << 8;
  return data | ram[addr + 1 & 0xffff] << 0;
}

auto BusCPU::writeByte(uint24 addr, uint16 data) -> void {
  if(addr < 0x400000) return cartridge.write(addr & ~1, data << 8 | data << 0);
  if(addr < 0xa00000) return;
  if(addr < 0xa10000) return;
  if(addr < 0xa10020) return writeIO(addr, data);
  if(addr < 0xc00000) return;
  if(addr < 0xe00000) return vdp.write(addr & ~1, data << 8 | data << 0);
  ram[addr & 0xffff] = data;
}

auto BusCPU::writeWord(uint24 addr, uint16 data) -> void {
  if(addr < 0x400000) return cartridge.write(addr, data);
  if(addr < 0xa00000) return;
  if(addr < 0xa10000) return;
  if(addr < 0xa10020) return writeIO(addr, data);
  if(addr < 0xc00000) return;
  if(addr < 0xe00000) return vdp.write(addr, data);
  ram[addr + 0 & 0xffff] = data >> 8;
  ram[addr + 1 & 0xffff] = data >> 0;
}

//

auto BusCPU::readIO(uint24 addr) -> uint16 {
  switch(addr & ~1) {
  case 0xa10002: return peripherals.controllerPort1->readData();
  case 0xa10004: return peripherals.controllerPort2->readData();
  case 0xa10006: return peripherals.extensionPort->readData();

  case 0xa10008: return peripherals.controllerPort1->readControl();
  case 0xa1000a: return peripherals.controllerPort2->readControl();
  case 0xa1000c: return peripherals.extensionPort->readControl();
  }

  return 0x0000;
}

auto BusCPU::writeIO(uint24 addr, uint16 data) -> void {
  switch(addr & ~1) {
  case 0xa10002: return peripherals.controllerPort1->writeData(data);
  case 0xa10004: return peripherals.controllerPort2->writeData(data);
  case 0xa10006: return peripherals.extensionPort->writeData(data);

  case 0xa10008: return peripherals.controllerPort1->writeControl(data);
  case 0xa1000a: return peripherals.controllerPort2->writeControl(data);
  case 0xa1000c: return peripherals.extensionPort->writeControl(data);
  }
}

//

auto BusAPU::read(uint16 addr) -> uint8 {
  return 0x00;
}

auto BusAPU::write(uint16 addr, uint8 data) -> void {
}

auto BusAPU::in(uint8 addr) -> uint8 {
  return 0x00;
}

auto BusAPU::out(uint8 addr, uint8 data) -> void {
}

}
