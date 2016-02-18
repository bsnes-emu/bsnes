#include <ws/ws.hpp>

namespace WonderSwan {

auto EEPROM::name() const -> string { return _name; }
auto EEPROM::data() -> uint16* { return _data; }
auto EEPROM::size() const -> uint { return _size; }

auto EEPROM::setName(string name) -> void {
  _name = name;
}

auto EEPROM::setSize(uint size) -> void {
  _size = bit::round(size);
}

auto EEPROM::erase() -> void {
  for(auto& word : _data) word = 0xffff;
}

auto EEPROM::power() -> void {
  r.latch = 0;
  r.address = 0;
  r.unknown = false;
  r.writeRequested = false;
  r.readRequested = false;
  r.writeCompleted = false;
  r.readCompleted = false;
  r.writeProtect = false;
}

auto EEPROM::read(uint port) -> uint8 {
  if(!_size) return 0x00;

  if(port == DataLo) return r.latch.byte(0);
  if(port == DataHi) return r.latch.byte(1);

  if(port == AddressLo) return r.address.byte(0);
  if(port == AddressHi) return r.address.byte(1);

  if(port == Status) return (
    1                << 7
  | r.unknown        << 6
  | r.writeRequested << 5
  | r.readRequested  << 4
  | r.writeCompleted << 1
  | r.readCompleted  << 0
  );

  return 0x00;
}

auto EEPROM::write(uint port, uint8 data) -> void {
  if(!_size) return;

  if(port == DataLo) {
    r.latch.byte(0) = data;
    return;
  }

  if(port == DataHi) {
    r.latch.byte(1) = data;
    return;
  }

  if(port == AddressLo) {
    r.address.byte(0) = data;
    return;
  }

  if(port == AddressHi) {
    r.address.byte(1) = data;
    return;
  }

  if(port == Command) {
    r.unknown        = data.bit(6);
    r.writeRequested = data.bit(5);
    r.readRequested  = data.bit(4);
    execute();
    return;
  }
}

auto EEPROM::execute() -> void {
  auto bits = bit::first(_size);
  auto address = r.address.bits(0, bits - 1);
  auto special = r.address.bits(bits - 2, bits - 1);
  auto command = r.address.bits(bits, bits + 1);
  auto start = r.address.bit(bits + 2);
  if(!start) return;

  //write disable
  if(command == 0 && special == 0) {
    r.writeProtect = true;
  }

  //write all
  if(command == 0 && special == 1 && !r.writeProtect) {
    for(auto n : range(_size)) _data[n] = r.latch;
  }

  //erase all
  if(command == 0 && special == 2 && !r.writeProtect) {
    for(auto n : range(_size)) _data[n] = 0xffff;
  }

  //write enable
  if(command == 0 && special == 3) {
    r.writeProtect = false;
  }

  //write word
  if(command == 1 && r.writeRequested && !r.writeProtect) {
    _data[address] = r.latch;
    r.writeRequested = false;
    r.writeCompleted = true;
  }

  //read word
  if(command == 2 && r.readRequested) {
    r.latch = _data[address];
    r.readRequested = false;
    r.readCompleted = true;
  }

  //erase word
  if(command == 3 && r.writeRequested && !r.writeProtect) {
    _data[address] = 0xffff;
    r.writeRequested = false;
    r.writeCompleted = true;
  }
}

}
