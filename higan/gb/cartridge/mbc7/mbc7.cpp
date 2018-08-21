#include "eeprom.cpp"
#include "serialization.cpp"

auto Cartridge::MBC7::load(Markup::Node document) -> void {
  eeprom.load(document);
}

auto Cartridge::MBC7::save(Markup::Node document) -> void {
  eeprom.save(document);
}

auto Cartridge::MBC7::main() -> void {
  eeprom.main();
}

auto Cartridge::MBC7::read(uint16 address) -> uint8 {
  if((address & 0xc000) == 0x0000) {  //$0000-3fff
    return cartridge.rom.read(address.bits(0,13));
  }

  if((address & 0xc000) == 0x4000) {  //$4000-7fff
    return cartridge.rom.read(io.rom.bank << 14 | address.bits(0,13));
  }

  if((address & 0xf000) == 0xa000) {  //$a000-afff
    if(!io.ram.enable[0] || !io.ram.enable[1]) return 0xff;

    switch(address.bits(4,7)) {
    case 2: return io.accelerometer.x.bits(0, 7);
    case 3: return io.accelerometer.x.bits(8,15);
    case 4: return io.accelerometer.y.bits(0, 7);
    case 5: return io.accelerometer.y.bits(8,15);
    case 6: return 0x00;  //z?
    case 7: return 0xff;  //z?
    case 8: return eeprom.readIO();
    }

    return 0xff;
  }

  return 0xff;
}

auto Cartridge::MBC7::write(uint16 address, uint8 data) -> void {
  if((address & 0xe000) == 0x0000) {  //$0000-1fff
    io.ram.enable[0] = data.bits(0,3) == 0xa;
    if(!io.ram.enable[0]) io.ram.enable[1] = false;
    return;
  }

  if((address & 0xe000) == 0x2000) {  //$2000-3fff
    io.rom.bank = data;
    if(!io.rom.bank) io.rom.bank = 1;
    return;
  }

  if((address & 0xe000) == 0x4000) {  //$4000-5fff
    if(!io.ram.enable[0]) return;
    io.ram.enable[1] = data == 0x40;
  }

  if((address & 0xf000) == 0xa000) {  //$a000-afff
    if(!io.ram.enable[0] || !io.ram.enable[1]) return;

    switch(address.bits(4,7)) {
    case 0: {
      if(data != 0x55) break;
      io.accelerometer.x = Center;
      io.accelerometer.y = Center;
      break;
    }

    case 1: {
      if(data != 0xaa) break;
      io.accelerometer.x = Center + platform->inputPoll(ID::Port::Hardware, ID::Device::Controls, 8);
      io.accelerometer.y = Center + platform->inputPoll(ID::Port::Hardware, ID::Device::Controls, 9);
      break;
    }

    case 8: {
      eeprom.writeIO(data);
      break;
    }
    }

    return;
  }
}

auto Cartridge::MBC7::power() -> void {
  eeprom.power();
  io = {};
}
