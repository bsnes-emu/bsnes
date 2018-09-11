//Microchip 93LCx6
//  93LC46 => 1024 cells => 128 x 8-bit or  64 x 16-bit
//  93LC56 => 2048 cells => 256 x 8-bit or 128 x 16-bit
//  93LC66 => 4096 cells => 512 x 8-bit or 256 x 16-bit

auto Cartridge::MBC7::EEPROM::load(Markup::Node document) -> void {
  for(auto& byte : data) byte = 0xff;
  size = 512;  //EEPROM size is in bytes
  width = 16;  //16-bit configuration

  if(auto memory = Emulator::Game::Memory{document["game/board/memory(type=EEPROM,content=Save)"]}) {
    if(memory.size == 128) size = 128;
    if(memory.size == 256) size = 256;
    if(memory.size == 512) size = 512;

    if(auto fp = platform->open(cartridge.pathID(), memory.name(), File::Read, File::Optional)) {
      fp->read(data, min(fp->size(), sizeof(data)));
    }
  }

  //note: the 93LC56 alone has an extra dummy address bit
  if(size == 128) input.addressLength = width == 16 ? 6 : 7;  //93LC46
  if(size == 256) input.addressLength = width == 16 ? 8 : 9;  //93LC56
  if(size == 512) input.addressLength = width == 16 ? 8 : 9;  //93LC66
  input.dataLength = width;
}

auto Cartridge::MBC7::EEPROM::save(Markup::Node document) -> void {
  if(auto memory = Emulator::Game::Memory{document["game/board/memory(type=EEPROM,content=Save)"]}) {
    if(auto fp = platform->open(cartridge.pathID(), memory.name(), File::Write)) {
      fp->write(data, size);
    }
  }
}

auto Cartridge::MBC7::EEPROM::main() -> void {
  //step by approximately one millisecond
  cartridge.step(cartridge.frequency() / 1000);

  //set during programming commands
  if(busy) busy--;
}

auto Cartridge::MBC7::EEPROM::power() -> void {
  select = 0;
  clock = 0;
  writable = 0;
  busy = 0;

  input.flush();
  output.flush();
}

auto Cartridge::MBC7::EEPROM::readIO() -> uint8 {
  uint8 data = 0b00'1111'00;
  data.bit(7) = select;
  data.bit(6) = clock;
  data.bit(1) = input.edge();
  if(!select) {
    data.bit(0) = 1;  //high-z when the chip is idle (not selected)
  } else if(busy) {
    data.bit(0) = 0;  //low when a programming command is in progress
  } else if(output.count) {
    data.bit(0) = output.edge();  //shift register data during read commands
  } else {
    data.bit(0) = 1;  //high-z during all other commands
  }
  return data;
}

auto Cartridge::MBC7::EEPROM::writeIO(uint8 data) -> void {
  //chip enters idle state on falling CS edge
  if(select && !data.bit(7)) return power();

  //chip leaves idle state on rising CS edge
  if(!(select = data.bit(7))) return;

  //input shift register clocks on rising edge
  if(!clock.raise(data.bit(6))) return;

  //read mode
  if(output.count && !data.bit(1)) {
    if(input.start() && *input.start() == 1) {
      if(input.opcode() && *input.opcode() == 0b10) {
        output.read();
        if(output.count == 0) {
          //sequential read mode
          input.increment();
          read();
        }
      }
    }
    return;
  }
  output.flush();

  input.write(data.bit(1));

  //wait for start
  if(!input.start()) return;
  uint start = *input.start();

  //start bit must be set
  if(start != 1) return input.flush();

  //wait for opcode
  if(!input.opcode()) return;
  uint opcode = *input.opcode();

  //wait for address
  if(!input.address()) return;

  if(opcode == 0b00) {
    auto mode = *input.mode();
    if(mode == 0b00) return writeDisable();
    if(mode == 0b01) return writeAll();
    if(mode == 0b10) return eraseAll();
    if(mode == 0b11) return writeEnable();
  }
  if(opcode == 0b01) return write();
  if(opcode == 0b10) return read();
  if(opcode == 0b11) return erase();
}

//

auto Cartridge::MBC7::EEPROM::read() -> void {
  uint address = *input.address() << (width == 16) & size - 1;
  output.flush();
  for(uint4 index : range(width)) {
    output.write(data[address + !index.bit(3)].bit(index.bits(0,2)));
  }
  output.write(0);  //reads have an extra dummy data bit
}

auto Cartridge::MBC7::EEPROM::write() -> void {
  if(!input.data()) return;  //wait for data
  if(!writable) return input.flush();
  uint address = *input.address() << (width == 16) & size - 1;
  for(uint4 index : range(width)) {
    data[address + !index.bit(3)].bit(index.bits(0,2)) = input.read();
  }
  busy = 4;  //milliseconds
  return input.flush();
}

auto Cartridge::MBC7::EEPROM::erase() -> void {
  if(!writable) return input.flush();
  uint address = *input.address() << (width == 16) & size - 1;
  for(uint index : range(width)) {
    data[address + index / 8].bit(index % 8) = 1;
  }
  busy = 4;  //milliseconds
  return input.flush();
}

auto Cartridge::MBC7::EEPROM::writeAll() -> void {
  if(!input.data()) return;  //wait for data
  if(!writable) return input.flush();
  auto word = *input.data();
  for(uint address = 0; address < 512;) {
    data[address++] = word.byte(width == 16);
    data[address++] = word.byte(0);
  }
  busy = 16;  //milliseconds
  return input.flush();
}

auto Cartridge::MBC7::EEPROM::eraseAll() -> void {
  if(!writable) return input.flush();
  for(auto& byte : data) byte = 0xff;
  busy = 8;  //milliseconds
  return input.flush();
}

auto Cartridge::MBC7::EEPROM::writeEnable() -> void {
  writable = true;
  return input.flush();
}

auto Cartridge::MBC7::EEPROM::writeDisable() -> void {
  writable = false;
  return input.flush();
}

//

auto Cartridge::MBC7::EEPROM::ShiftRegister::flush() -> void {
  value = 0;
  count = 0;
}

auto Cartridge::MBC7::EEPROM::ShiftRegister::edge() -> uint1 {
  return value.bit(0);
}

auto Cartridge::MBC7::EEPROM::ShiftRegister::read() -> uint1 {
  uint1 bit = value.bit(0);
  value >>= 1;
  count--;
  return bit;
}

auto Cartridge::MBC7::EEPROM::ShiftRegister::write(uint1 bit) -> void {
  value <<= 1;
  value.bit(0) = bit;
  count++;
}

//

auto Cartridge::MBC7::EEPROM::InputShiftRegister::start() -> maybe<uint1> {
  if(count < 1) return {};
  return {value >> count - 1 & 1};
}

auto Cartridge::MBC7::EEPROM::InputShiftRegister::opcode() -> maybe<uint2> {
  if(count < 1 + 2) return {};
  return {value >> count - 3 & 3};
}

auto Cartridge::MBC7::EEPROM::InputShiftRegister::mode() -> maybe<uint2> {
  if(count < 1 + 2 + addressLength) return {};
  return {value >> count - 5 & 3};
}

auto Cartridge::MBC7::EEPROM::InputShiftRegister::address() -> maybe<uint9> {
  if(count < 1 + 2 + addressLength) return {};
  return {value >> count - (3 + addressLength) & (1 << addressLength) - 1};
}

auto Cartridge::MBC7::EEPROM::InputShiftRegister::data() -> maybe<uint16> {
  if(count < 1 + 2 + addressLength + dataLength) return {};
  return {value >> count - (3 + addressLength + dataLength) & (1 << dataLength) - 1};
}

auto Cartridge::MBC7::EEPROM::InputShiftRegister::increment() -> void {
  value.bits(0, addressLength - 1)++;
}
