#include <sfc/sfc.hpp>

namespace SuperFamicom {

CPU cpu;
#include "dma.cpp"
#include "memory.cpp"
#include "io.cpp"
#include "timing.cpp"
#include "irq.cpp"
#include "joypad.cpp"
#include "serialization.cpp"

auto CPU::interruptPending() const -> bool { return status.interruptPending; }
auto CPU::pio() const -> uint8 { return io.pio; }
auto CPU::joylatch() const -> bool { return io.joypadStrobeLatch; }

CPU::CPU() {
  PPUcounter::scanline = {&CPU::scanline, this};
}

auto CPU::Enter() -> void {
  while(true) scheduler.synchronize(), cpu.main();
}

auto CPU::main() -> void {
  if(status.interruptPending) {
    status.interruptPending = false;
    if(status.nmiPending) {
      status.nmiPending = false;
      r.vector = r.e ? 0xfffa : 0xffea;
      interrupt();
    } else if(status.irqPending) {
      status.irqPending = false;
      r.vector = r.e ? 0xfffe : 0xffee;
      interrupt();
    } else if(status.resetPending) {
      status.resetPending = false;
      step(132);
      r.vector = 0xfffc;
      interrupt();
    } else if(status.powerPending) {
      status.powerPending = false;
      step(186);
      r.pc.l = bus.read(0xfffc, r.mdr);
      r.pc.h = bus.read(0xfffd, r.mdr);
    }
  }

  instruction();
}

auto CPU::load(Markup::Node node) -> bool {
  version = max(1, min(2, node["cpu/version"].natural()));
  return true;
}

auto CPU::power() -> void {
  create(Enter, system.colorburst() * 6.0);
  coprocessors.reset();
  PPUcounter::reset();

  function<auto (uint24, uint8) -> uint8> reader;
  function<auto (uint24, uint8) -> void> writer;

  reader = {&CPU::readAPU, this};
  writer = {&CPU::writeAPU, this};
  bus.map(reader, writer, "00-3f,80-bf:2140-217f");

  reader = {&CPU::readCPU, this};
  writer = {&CPU::writeCPU, this};
  bus.map(reader, writer, "00-3f,80-bf:2180-2183,4016-4017,4200-421f");

  reader = {&CPU::readDMA, this};
  writer = {&CPU::writeDMA, this};
  bus.map(reader, writer, "00-3f,80-bf:4300-437f");

  reader = [](uint24 addr, uint8) -> uint8 { return cpu.wram[addr]; };
  writer = [](uint24 addr, uint8 data) -> void { cpu.wram[addr] = data; };
  bus.map(reader, writer, "00-3f,80-bf:0000-1fff", 0x2000);
  bus.map(reader, writer, "7e-7f:0000-ffff", 0x20000);

  for(auto& byte : wram) byte = random(0x55);

  //CPU
  r.pc     = 0x000000;
  r.a      = 0x0000;
  r.x      = 0x0000;
  r.y      = 0x0000;
  r.s      = 0x01ff;
  r.d      = 0x0000;
  r.db     = 0x00;
  r.p      = 0x34;
  r.e      = 1;
  r.mdr    = 0x00;
  r.wai    = false;
  r.vector = 0xfffc;  //reset vector address

  //DMA
  for(auto& channel : this->channel) {
    channel.dmaEnabled = false;
    channel.hdmaEnabled = false;

    channel.direction = 1;
    channel.indirect = true;
    channel.unused = true;
    channel.reverseTransfer = true;
    channel.fixedTransfer = true;
    channel.transferMode = 7;

    channel.targetAddress = 0xff;

    channel.sourceAddress = 0xffff;
    channel.sourceBank = 0xff;

    channel.transferSize = 0xffff;
    channel.indirectBank = 0xff;

    channel.hdmaAddress = 0xffff;
    channel.lineCounter = 0xff;
    channel.unknown = 0xff;

    channel.hdmaCompleted = false;
    channel.hdmaDoTransfer = false;
  }

  //$2140-217f
  for(auto& port : io.port) port = 0x00;

  //$2181-$2183
  io.wramAddress = 0x000000;

  //$4016-$4017
  io.joypadStrobeLatch = 0;

  //$4200
  io.nmiEnabled = false;
  io.hirqEnabled = false;
  io.virqEnabled = false;
  io.autoJoypadPoll = false;

  //$4201
  io.pio = 0xff;

  //$4202-$4203
  io.wrmpya = 0xff;
  io.wrmpyb = 0xff;

  //$4204-$4206
  io.wrdiva = 0xffff;
  io.wrdivb = 0xff;

  //$4207-$420a
  io.hirqPos = 0x01ff;
  io.virqPos = 0x01ff;

  //$420d
  io.romSpeed = 8;

  //$4214-$4217
  io.rddiv = 0x0000;
  io.rdmpy = 0x0000;

  //$4218-$421f
  io.joy1 = 0x0000;
  io.joy2 = 0x0000;
  io.joy3 = 0x0000;
  io.joy4 = 0x0000;

  //ALU
  alu.mpyctr = 0;
  alu.divctr = 0;
  alu.shift = 0;

  //Pipe
  pipe.valid = false;
  pipe.addr = 0;
  pipe.data = 0;

  //Timing
  status.clockCount = 0;
  status.lineClocks = lineclocks();

  status.irqLock = false;
  status.dramRefreshPosition = (version == 1 ? 530 : 538);
  status.dramRefreshed = false;

  status.hdmaInitPosition = (version == 1 ? 12 + 8 - dmaCounter() : 12 + dmaCounter());
  status.hdmaInitTriggered = false;

  status.hdmaPosition = 1104;
  status.hdmaTriggered = false;

  status.nmiValid      = false;
  status.nmiLine       = false;
  status.nmiTransition = false;
  status.nmiPending    = false;
  status.nmiHold       = false;

  status.irqValid      = false;
  status.irqLine       = false;
  status.irqTransition = false;
  status.irqPending    = false;
  status.irqHold       = false;

  status.powerPending = true;
  status.resetPending = false;
  status.interruptPending = true;

  status.dmaActive   = false;
  status.dmaCounter  = 0;
  status.dmaClocks   = 0;
  status.dmaPending  = false;
  status.hdmaPending = false;
  status.hdmaMode    = 0;

  status.autoJoypadActive  = false;
  status.autoJoypadLatch   = false;
  status.autoJoypadCounter = 0;
  status.autoJoypadClock   = 0;
}

}
