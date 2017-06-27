#include <md/md.hpp>

namespace MegaDrive {

BusCPU busCPU;
BusAPU busAPU;
#include "serialization.cpp"

auto BusCPU::readByte(uint24 addr) -> uint16 {
  if(addr >= 0x000000 && addr <= 0x3fffff) return cartridge.read(addr & ~1).byte(!addr.bit(0));
  if(addr >= 0xa00000 && addr <= 0xa0ffff) return busAPU.granted() ? busAPU.read(addr) : (uint8)0x0000;
  if(addr >= 0xa10000 && addr <= 0xa10fff) return readIO(addr & ~0xff00);
  if(addr >= 0xa11000 && addr <= 0xa11fff) return readIO(addr & ~0x00ff);
  if(addr >= 0xa13000 && addr <= 0xa130ff) return cartridge.readIO(addr);
  if(addr >= 0xc00000 && addr <= 0xdfffff) return vdp.read(addr & ~1).byte(!addr.bit(0));
  if(addr >= 0xe00000 && addr <= 0xffffff) {
    return ram[addr & 0xffff];
  }
  return 0x0000;
}

auto BusCPU::readWord(uint24 addr) -> uint16 {
  if(addr >= 0x000000 && addr <= 0x3fffff) return cartridge.read(addr);
  if(addr >= 0xa00000 && addr <= 0xa0ffff) return busAPU.granted() ? busAPU.read(addr) : (uint8)0x0000;
  if(addr >= 0xa10000 && addr <= 0xa10fff) return readIO(addr & ~0xff00) << 0;
  if(addr >= 0xa11000 && addr <= 0xa11fff) return readIO(addr & ~0x00ff) << 8;
  if(addr >= 0xa13000 && addr <= 0xa130ff) return cartridge.readIO(addr);
  if(addr >= 0xc00000 && addr <= 0xdfffff) return vdp.read(addr);
  if(addr >= 0xe00000 && addr <= 0xffffff) {
    uint16 data = ram[addr + 0 & 0xffff] << 8;
    return data | ram[addr + 1 & 0xffff] << 0;
  }
  return 0x0000;
}

auto BusCPU::writeByte(uint24 addr, uint16 data) -> void {
  if(addr >= 0x000000 && addr <= 0x3fffff) return cartridge.write(addr & ~1, data << 8 | data << 0);
  if(addr >= 0xa00000 && addr <= 0xa0ffff) return busAPU.granted() ? busAPU.write(addr, data) : (void)0;
  if(addr >= 0xa10000 && addr <= 0xa10fff) return writeIO(addr & ~0xff00, data);
  if(addr >= 0xa11000 && addr <= 0xa11fff) return writeIO(addr & ~0x00ff, data);
  if(addr >= 0xa13000 && addr <= 0xa130ff) return cartridge.writeIO(addr, data);
  if(addr >= 0xc00000 && addr <= 0xc0000f) return vdp.write(addr & ~1, data << 8 | data << 0);
  if(addr >= 0xc00010 && addr <= 0xc00017) return psg.write(data);
  if(addr >= 0xe00000 && addr <= 0xffffff) {
    ram[addr & 0xffff] = data;
    return;
  }
}

auto BusCPU::writeWord(uint24 addr, uint16 data) -> void {
  if(addr >= 0x000000 && addr <= 0x3fffff) return cartridge.write(addr, data);
  if(addr >= 0xa00000 && addr <= 0xa0ffff) return busAPU.granted() ? busAPU.write(addr, data) : (void)0;
  if(addr >= 0xa10000 && addr <= 0xa10fff) return writeIO(addr & ~0xff00, data >> 0);
  if(addr >= 0xa11000 && addr <= 0xa11fff) return writeIO(addr & ~0x00ff, data >> 8);
  if(addr >= 0xa13000 && addr <= 0xa130ff) return cartridge.writeIO(addr, data);
  if(addr >= 0xc00000 && addr <= 0xc0000f) return vdp.write(addr, data);
  if(addr >= 0xc00010 && addr <= 0xc00017) return psg.write(data);
  if(addr >= 0xe00000 && addr <= 0xffffff) {
    ram[addr + 0 & 0xffff] = data >> 8;
    ram[addr + 1 & 0xffff] = data >> 0;
    return;
  }
}

//

auto BusCPU::readIO(uint24 addr) -> uint16 {
  switch(addr & ~1) {
  case 0xa10000: return (
    !Region::NTSCJ() << 7  //0 = domestic (Japan); 1 = export
  | Region::PAL() << 6     //0 = NTSC; 1 = PAL
  | 1 << 5                 //0 = Mega CD connected; 1 = no expansion connected
  | 0 << 0                 //0 = Model 1; 1 = Model 2+
  );

  case 0xa10002: return controllerPort1.readData();
  case 0xa10004: return controllerPort2.readData();
  case 0xa10006: return extensionPort.readData();

  case 0xa10008: return controllerPort1.readControl();
  case 0xa1000a: return controllerPort2.readControl();
  case 0xa1000c: return extensionPort.readControl();

  case 0xa11100: return !busAPU.granted();
  }

  return 0x0000;
}

auto BusCPU::writeIO(uint24 addr, uint16 data) -> void {
  switch(addr & ~1) {
  case 0xa10002: return controllerPort1.writeData(data);
  case 0xa10004: return controllerPort2.writeData(data);
  case 0xa10006: return extensionPort.writeData(data);

  case 0xa10008: return controllerPort1.writeControl(data);
  case 0xa1000a: return controllerPort2.writeControl(data);
  case 0xa1000c: return extensionPort.writeControl(data);

  case 0xa11100: return busAPU.request(data.bit(0));
  case 0xa11200: return apu.enable(data.bit(0));
  }
}

//

auto BusAPU::read(uint16 addr) -> uint8 {
  if((addr & 0xe000) == 0x0000) {
    return ram[addr];
  }

  if(addr == 0x4000) return ym2612.readStatus();
  if(addr == 0x4001) return ym2612.readStatus();
  if(addr == 0x4002) return ym2612.readStatus();
  if(addr == 0x4003) return ym2612.readStatus();

  if((addr & 0x8000) == 0x8000) {
    return cartridge.read(bank << 15 | (addr & 0x7ffe)).byte(!addr.bit(0));
  }
}

auto BusAPU::write(uint16 addr, uint8 data) -> void {
  if((addr & 0xe000) == 0x0000) {
    ram[addr] = data;
    return;
  }

  if(addr == 0x4000) return ym2612.writeAddress(0 << 8 | data);
  if(addr == 0x4001) return ym2612.writeData(data);
  if(addr == 0x4002) return ym2612.writeAddress(1 << 8 | data);
  if(addr == 0x4003) return ym2612.writeData(data);

  if(addr == 0x6000) {
    //1-bit shift register
    bank = data.bit(0) << 8 | bank >> 1;
    return;
  }

  if(addr == 0x7f11) return psg.write(data);
  if(addr == 0x7f13) return psg.write(data);
  if(addr == 0x7f15) return psg.write(data);
  if(addr == 0x7f17) return psg.write(data);

  if((addr & 0x8000) == 0x8000) {
    //todo: do 8-bit writes mirror to 16-bits?
    return cartridge.write(bank << 15 | (addr & 0x7ffe), data << 8 | data << 0);
  }
}

//unused on Mega Drive
auto BusAPU::in(uint8 addr) -> uint8 {
  return 0x00;
}

//unused on Mega Drive
auto BusAPU::out(uint8 addr, uint8 data) -> void {
}

}
