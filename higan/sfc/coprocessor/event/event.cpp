#include <sfc/sfc.hpp>

namespace SuperFamicom {

Event event;

auto Event::Enter() -> void {
  while(true) scheduler.synchronize(), event.main();
}

auto Event::main() -> void {
  if(scoreActive && scoreSecondsRemaining) {
    if(--scoreSecondsRemaining == 0) {
      scoreActive = false;
    }
  }

  if(timerActive && timerSecondsRemaining) {
    if(--timerSecondsRemaining == 0) {
      timerActive = false;
      status |= 0x02;  //time over
      scoreActive = true;
      scoreSecondsRemaining = 5;
    }
  }

  step(1);
  synchronize(cpu);
}

auto Event::init() -> void {
}

auto Event::load() -> void {
}

auto Event::unload() -> void {
  rom[0].reset();
  rom[1].reset();
  rom[2].reset();
  rom[3].reset();
  ram.reset();
}

auto Event::power() -> void {
}

auto Event::reset() -> void {
  create(Event::Enter, 1);

  for(auto n : range(ram.size())) ram.write(n, 0x00);
  status = 0x00;
  select = 0x00;
  timerActive = false;
  scoreActive = false;
  timerSecondsRemaining = 0;
  scoreSecondsRemaining = 0;
}

auto Event::mcuRead(uint24 addr, uint8 data) -> uint8 {
  if(board == Board::CampusChallenge92) {
    uint id = 0;
    if(select == 0x09) id = 1;
    if(select == 0x05) id = 2;
    if(select == 0x03) id = 3;
    if((addr & 0x808000) == 0x808000) id = 0;

    if(addr & 0x008000) {
      addr = ((addr & 0x7f0000) >> 1) | (addr & 0x7fff);
      return rom[id].read(bus.mirror(addr, rom[id].size()), data);
    }
  }

  if(board == Board::Powerfest94) {
    uint id = 0;
    if(select == 0x09) id = 1;
    if(select == 0x0c) id = 2;
    if(select == 0x0a) id = 3;
    if((addr & 0x208000) == 0x208000) id = 0;

    if(addr & 0x400000) {
      addr &= 0x3fffff;
      return rom[id].read(bus.mirror(addr, rom[id].size()), data);
    }

    if(addr & 0x008000) {
      addr &= 0x1fffff;
      if(id != 2) addr = ((addr & 0x1f0000) >> 1) | (addr & 0x7fff);
      return rom[id].read(bus.mirror(addr, rom[id].size()), data);
    }
  }

  return data;
}

auto Event::mcuWrite(uint24 addr, uint8 data) -> void {
}

auto Event::read(uint24 addr, uint8 data) -> uint8 {
  if(addr == 0x106000 || addr == 0xc00000) {
    return status;
  }
  return data;
}

auto Event::write(uint24 addr, uint8 data) -> void {
  if(addr == 0x206000 || addr == 0xe00000) {
    select = data;
    if(timer && data == 0x09) {
      timerActive = true;
      timerSecondsRemaining = timer;
    }
  }
}

auto Event::serialize(serializer& s) -> void {
  Thread::serialize(s);
  s.array(ram.data(), ram.size());
  s.integer(status);
  s.integer(select);
  s.integer(timerActive);
  s.integer(scoreActive);
  s.integer(timerSecondsRemaining);
  s.integer(scoreSecondsRemaining);
}

}
