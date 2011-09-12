#include <nes/nes.hpp>

namespace NES {

APU apu;

void APU::Main() {
  apu.main();
}

void APU::main() {
  while(true) {
    system.interface->audio_sample(0);
    tick();
  }
}

void APU::tick() {
  clock += 12;
  if(clock >= 0) co_switch(cpu.thread);
}

void APU::power() {
  reset();
}

void APU::reset() {
  Processor::create(APU::Main, 21477272);
}

uint8 APU::read(uint16 addr) {
}

void APU::write(uint16 addr, uint8 data) {
  switch(addr) {
  case 0x4000:
    pulse1.duty = (data >> 6);
    pulse1.halt = data & 0x20;
    pulse1.envelope = data & 0x1f;
    break;
  case 0x4002:
    pulse1.timer = (pulse1.timer & 0x0700) | ((data & 0xff) << 0);
    break;
  case 0x4003:
    pulse1.timer = (pulse1.timer & 0x00ff) | ((data & 0x07) << 8);
    pulse1.length = (data >> 3);
    break;
  case 0x4004:
    pulse2.duty = (data >> 6);
    pulse2.halt = data & 0x20;
    pulse2.envelope = data & 0x1f;
    break;
  case 0x4006:
    pulse2.timer = (pulse2.timer & 0x0700) | ((data & 0xff) << 0);
    break;
  case 0x4007:
    pulse2.timer = (pulse2.timer & 0x00ff) | ((data & 0x07) << 8);
    pulse2.length = (data >> 3);
    break;
  case 0x4008:
    triangle.control = data & 0x80;
    triangle.reload = data & 0x7f;
    break;
  case 0x400a:
    triangle.timer = (triangle.timer & 0x0700) | ((data & 0xff) << 0);
    break;
  case 0x400b:
    triangle.timer = (triangle.timer & 0x00ff) | ((data & 0x07) << 8);
    triangle.length = (data >> 3);
    break;
  case 0x400c:
    noise.halt = data & 0x20;
    noise.envelope = data & 0x1f;
    break;
  case 0x400e:
    noise.mode = data & 0x80;
    noise.period = data & 0x0f;
    break;
  case 0x400f:
    noise.length = (data >> 3);
    break;
  case 0x4010:
    dmc.irq = data & 0x80;
    dmc.loop = data & 0x40;
    dmc.rate = data & 0x0f;
    break;
  case 0x4011:
    dmc.direct_load = data & 0x7f;
    break;
  case 0x4012:
    dmc.sample_addr = 0xc000 | (data << 6);
    break;
  case 0x4013:
    dmc.sample_length = (data << 4) | 1;
    break;
  }
}

}
