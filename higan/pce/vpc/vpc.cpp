#include <pce/pce.hpp>

namespace PCEngine {

VPC vpc;

auto VPC::bus(uint hclock) -> uint9 {
  auto bus0 = vdc0.bus();
  auto bus1 = vdc1.bus();

  auto color0 = bus0.bits(0,3);
  auto color1 = bus1.bits(0,3);

  auto palette0 = bus0.bits(4,7);
  auto palette1 = bus1.bits(4,7);

  auto mode0 = bus0.bit(8);
  auto mode1 = bus1.bit(8);

  //todo: I am unsure how the window coordinates relate to raw screen pixels ...
  bool window0 = window[0] >= 64 && (window[0] - 64) >= hclock;
  bool window1 = window[1] >= 64 && (window[1] - 64) >= hclock;

  uint2 mode;
  if(!window0 && !window1) mode = 1;
  if( window0 && !window1) mode = 0;
  if(!window0 &&  window1) mode = 3;
  if( window0 &&  window1) mode = 2;

  auto enableVDC0 = settings[mode].enableVDC0;
  auto enableVDC1 = settings[mode].enableVDC1;
  auto priority = settings[mode].priority;

  //todo: I am unsure how this should work ...
  if(priority == 0 || priority == 3) {
    if(color1) return bus1;
    return bus0;
  }

  if(priority == 1) {
    if(color1) return bus1;
    return bus0;
  }

  if(priority == 2) {
    if(color1) return bus1;
    return bus0;
  }

  unreachable;
}

auto VPC::power() -> void {
  settings[0].enableVDC0 = true;
  settings[0].enableVDC1 = false;
  settings[0].priority = 0;

  settings[1].enableVDC0 = true;
  settings[1].enableVDC1 = false;
  settings[1].priority = 0;

  settings[2].enableVDC0 = true;
  settings[2].enableVDC1 = false;
  settings[2].priority = 0;

  settings[3].enableVDC0 = true;
  settings[3].enableVDC1 = false;
  settings[3].priority = 0;

  window[0] = 0;
  window[1] = 1;

  select = 0;
}

auto VPC::read(uint5 addr) -> uint8 {
  if(addr >= 0x00 && addr <= 0x07) return vdc0.read(addr);
  if(addr >= 0x10 && addr <= 0x17) return vdc1.read(addr);
  if(addr >= 0x18 && addr <= 0x1f) return 0xff;

  if(addr == 0x08) {
    return (
      settings[0].enableVDC0 << 0
    | settings[0].enableVDC1 << 1
    | settings[0].priority << 2
    | settings[1].enableVDC0 << 4
    | settings[1].enableVDC1 << 5
    | settings[1].priority << 6
    );
  }

  if(addr == 0x09) {
    return (
      settings[2].enableVDC0 << 0
    | settings[2].enableVDC1 << 1
    | settings[2].priority << 2
    | settings[3].enableVDC0 << 4
    | settings[3].enableVDC1 << 5
    | settings[3].priority << 6
    );
  }

  if(addr == 0x0a) return window[0].bits(0,7);
  if(addr == 0x0b) return window[0].bits(8,9);
  if(addr == 0x0c) return window[1].bits(0,7);
  if(addr == 0x0d) return window[1].bits(8,9);
  if(addr == 0x0e) return 0x00;  //select is not readable
  if(addr == 0x0f) return 0x00;  //unused

  unreachable;
}

auto VPC::write(uint5 addr, uint8 data) -> void {
  if(addr >= 0x00 && addr <= 0x07) return vdc0.write(addr, data);
  if(addr >= 0x10 && addr <= 0x17) return vdc1.write(addr, data);
  if(addr >= 0x18 && addr <= 0x1f) return;

  if(addr == 0x08) {
    settings[0].enableVDC0 = data.bit(0);
    settings[0].enableVDC1 = data.bit(1);
    settings[0].priority = data.bits(2,3);
    settings[1].enableVDC0 = data.bit(4);
    settings[1].enableVDC1 = data.bit(5);
    settings[1].priority = data.bits(6,7);
    return;
  }

  if(addr == 0x09) {
    settings[2].enableVDC0 = data.bit(0);
    settings[2].enableVDC1 = data.bit(1);
    settings[2].priority = data.bits(2,3);
    settings[3].enableVDC0 = data.bit(4);
    settings[3].enableVDC1 = data.bit(5);
    settings[3].priority = data.bits(6,7);
    return;
  }

  if(addr == 0x0a) {
    window[0].bits(0,7) = data.bits(0,7);
    return;
  }

  if(addr == 0x0b) {
    window[0].bits(8,9) = data.bits(0,1);
    return;
  }

  if(addr == 0x0c) {
    window[1].bits(0,7) = data.bits(0,7);
    return;
  }

  if(addr == 0x0d) {
    window[1].bits(8,9) = data.bits(0,1);
    return;
  }

  if(addr == 0x0e) {
    select = data.bit(0);
    return;
  }

  if(addr == 0x0f) {
    //unused
    return;
  }
}

auto VPC::store(uint2 addr, uint8 data) -> void {
  if(select == 0) return vdc0.write(addr, data);
  if(select == 1) return vdc1.write(addr, data);
}

}
