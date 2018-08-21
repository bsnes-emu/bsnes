//Microchip 93LCx6
//  93LC46 => 1024 cells => 128 x 8-bit or  64 x 16-bit
//  93LC56 => 2048 cells => 256 x 8-bit or 128 x 16-bit
//  93LC66 => 4096 cells => 512 x 8-bit or 256 x 16-bit

auto Cartridge::MBC7::EEPROM::load(Markup::Node document) -> void {
  for(auto& byte : data) byte = 0xff;
  size = 4096;  //EEPROM size is in bits
  width = 16;   //16-bit configuration

  if(auto memory = Emulator::Game::Memory{document["game/board/memory(type=EEPROM,content=Save)"]}) {
    if(memory.size == 128) size = 1024;  //manifest size is in bytes
    if(memory.size == 256) size = 2048;
    if(memory.size == 512) size = 4096;

    if(auto fp = platform->open(cartridge.pathID(), memory.name(), File::Read, File::Optional)) {
      fp->read(data, min(fp->size(), sizeof(data)));
    }
  }

  command.length = 3;
  if(size == 1024) address.length = width == 8 ? 6 : 7;
  if(size == 2048) address.length = width == 8 ? 7 : 8;
  if(size == 4096) address.length = width == 8 ? 8 : 9;
  input.length = width;
  output.length = 1 + width;  //there is an extra zero dummy bit on reads
}

auto Cartridge::MBC7::EEPROM::save(Markup::Node document) -> void {
  if(auto memory = Emulator::Game::Memory{document["game/board/memory(type=EEPROM,content=Save)"]}) {
    if(auto fp = platform->open(cartridge.pathID(), memory.name(), File::Write)) {
      fp->write(data, size >> 3);  //bytes -> bits
    }
  }
}

auto Cartridge::MBC7::EEPROM::main() -> void {
  //step by approximately one millisecond
  cartridge.step(cartridge.frequency() / 1000);

  //set during programming commands
  if(busy) busy--;
}

auto Cartridge::MBC7::EEPROM::power(bool reset) -> void {
  if(!reset) {
    select = 0;
    writable = 0;
  }

  clock = 0;
  busy = 0;

  command.flush();
  address.flush();
  input.flush();
  output.flush();
}

auto Cartridge::MBC7::EEPROM::readIO() -> uint8 {
  uint8 data = 0b00'1111'00;
  data.bit(7) = select;
  data.bit(6) = clock;
  data.bit(1) = 1;
  if(!select) {
    data.bit(0) = 1;  //high-z when the chip is idle (not selected)
  } else if(busy) {
    data.bit(0) = 0;  //low when a programming command is in progress
  } else if(output.count) {
    data.bit(0) = output.peek();  //shift register data during read commands
  } else {
    data.bit(0) = 1;  //high-z during all other commands
  }
  return data;
}

auto Cartridge::MBC7::EEPROM::writeIO(uint8 data) -> void {
  //bring chip out of idle state on rising CS edge
  if(select.raise(data.bit(7))) return power(true);

  //do nothing if chip is idle
  if(!select) return;

  //shift register clocks on rising edge
  if(!clock.raise(data.bit(6))) return;

  //sequential read mode
  if(output.count && !data.bit(1)) {
    output.read();
    if(output.count == 0) {
      address.value++;
      read();
    }
    return;
  }
  output.flush();

  //wait for start bit to be set
  if(command.count == 0 && !data.bit(1)) return;

  //waiting on command?
  if(command.count < command.length) {
    command.write(data.bit(1));
    if(command.count < command.length) return;

    return address.flush();
  }

  //waiting on address bits?
  if(address.count < address.length) {
    address.write(data.bit(1));
    if(address.count < address.length) return;

    uint3 opcode = command.bits(0, command.length - 1);
    if(opcode == 0b100) {
      uint2 mode = address.bits(address.length - 2, address.length - 1);
      if(mode == 0b00) return writeDisable();
      if(mode == 0b01) return input.flush();  //writeAll
      if(mode == 0b10) return eraseAll();
      if(mode == 0b11) return writeEnable();
    }
    if(opcode == 0b101) return input.flush();  //write
    if(opcode == 0b110) return read();
    if(opcode == 0b111) return erase();
    return;
  }

  //waiting on data bits from a write or writeAll command?
  if(input.count < input.length) {  //block new commands and inputs until the next clock edge
    input.write(data.bit(1));
    if(input.count < input.length) return;

    uint3 opcode = command.bits(0, command.length - 1);
    if(opcode == 0b101) return write();
    if(opcode == 0b100) return writeAll();
    return;
  }
}

//

auto Cartridge::MBC7::EEPROM::read() -> void {
  command.flush();
  auto address = this->address.value << (width == 16) & (size >> 3) - 1;
  output.value = 0;
  if(width >=  8) output.value |= data[address++] << 8;
  if(width >= 16) output.value |= data[address++] << 0;
  output.count = output.length;
}

auto Cartridge::MBC7::EEPROM::write() -> void {
  command.flush();
  if(!writable) return;
  auto address = this->address.value << (width == 16) & (size >> 3) - 1;
  if(width >=  8) data[address++] = input.value >> 8;
  if(width >= 16) data[address++] = input.value >> 0;
  input.flush();
  busy = 4;  //ms
}

auto Cartridge::MBC7::EEPROM::erase() -> void {
  command.flush();
  if(!writable) return;
  auto address = this->address.value << (width == 16) & (size >> 3) - 1;
  if(width >=  8) data[address++] = 0xff;
  if(width >= 16) data[address++] = 0xff;
  busy = 4;  //ms
}

auto Cartridge::MBC7::EEPROM::writeAll() -> void {
  command.flush();
  if(!writable) return;
  uint8 lo = input.byte(0);
  uint8 hi = input.byte(width == 16);
  for(uint address = 0; address < 512;) {
    data[address++] = hi;
    data[address++] = lo;
  }
  input.flush();
  busy = 16;  //ms
}

auto Cartridge::MBC7::EEPROM::eraseAll() -> void {
  command.flush();
  if(!writable) return;
  for(uint address; address < 512;) {
    data[address++] = 0xff;
    data[address++] = 0xff;
  }
  busy = 8;  //ms
}

auto Cartridge::MBC7::EEPROM::writeEnable() -> void {
  command.flush();
  writable = true;
}

auto Cartridge::MBC7::EEPROM::writeDisable() -> void {
  command.flush();
  writable = false;
}

//

auto Cartridge::MBC7::EEPROM::ShiftRegister::flush() -> void {
  value = 0;
  count = 0;
}

//read the current bit in the shift register without clocking it
auto Cartridge::MBC7::EEPROM::ShiftRegister::peek() -> bool {
  return value.bit(length - 1);
}

auto Cartridge::MBC7::EEPROM::ShiftRegister::read() -> bool {
  bool bit = value.bit(length - 1);
  value <<= 1;
  if(count) count--;
  return bit;
}

auto Cartridge::MBC7::EEPROM::ShiftRegister::write(bool bit) -> void {
  value <<= 1;
  value |= bit;
  count++;
}
