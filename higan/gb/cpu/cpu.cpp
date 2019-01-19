#include <gb/gb.hpp>

namespace GameBoy {

#include "io.cpp"
#include "memory.cpp"
#include "timing.cpp"
#include "serialization.cpp"
CPU cpu;

auto CPU::Enter() -> void {
  while(true) scheduler.synchronize(), cpu.main();
}

auto CPU::main() -> void {
  interruptTest();
  instruction();
}

auto CPU::raise(CPU::Interrupt id) -> void {
  if(id == Interrupt::Vblank) {
    status.interruptRequestVblank = 1;
    if(status.interruptEnableVblank) r.halt = false;
  }

  if(id == Interrupt::Stat) {
    status.interruptRequestStat = 1;
    if(status.interruptEnableStat) r.halt = false;
  }

  if(id == Interrupt::Timer) {
    status.interruptRequestTimer = 1;
    if(status.interruptEnableTimer) r.halt = false;
  }

  if(id == Interrupt::Serial) {
    status.interruptRequestSerial = 1;
    if(status.interruptEnableSerial) r.halt = false;
  }

  if(id == Interrupt::Joypad) {
    status.interruptRequestJoypad = 1;
    if(status.interruptEnableJoypad) r.halt = r.stop = false;
  }
}

auto CPU::interruptTest() -> void {
  if(!r.ime) return;

  if(status.interruptRequestVblank && status.interruptEnableVblank) {
    status.interruptRequestVblank = 0;
    return interrupt(0x0040);
  }

  if(status.interruptRequestStat && status.interruptEnableStat) {
    status.interruptRequestStat = 0;
    return interrupt(0x0048);
  }

  if(status.interruptRequestTimer && status.interruptEnableTimer) {
    status.interruptRequestTimer = 0;
    return interrupt(0x0050);
  }

  if(status.interruptRequestSerial && status.interruptEnableSerial) {
    status.interruptRequestSerial = 0;
    return interrupt(0x0058);
  }

  if(status.interruptRequestJoypad && status.interruptEnableJoypad) {
    status.interruptRequestJoypad = 0;
    return interrupt(0x0060);
  }
}

auto CPU::stop() -> bool {
  if(status.speedSwitch) {
    status.speedSwitch = 0;
    status.speedDouble ^= 1;
    if(status.speedDouble == 0) setFrequency(4 * 1024 * 1024);
    if(status.speedDouble == 1) setFrequency(8 * 1024 * 1024);
    return true;
  }
  return false;
}

auto CPU::power() -> void {
  create(Enter, 4 * 1024 * 1024);
  SM83::power();

  for(uint n = 0xc000; n <= 0xdfff; n++) bus.mmio[n] = this;  //WRAM
  for(uint n = 0xe000; n <= 0xfdff; n++) bus.mmio[n] = this;  //WRAM (mirror)
  for(uint n = 0xff80; n <= 0xfffe; n++) bus.mmio[n] = this;  //HRAM

  bus.mmio[0xff00] = this;  //JOYP
  bus.mmio[0xff01] = this;  //SB
  bus.mmio[0xff02] = this;  //SC
  bus.mmio[0xff04] = this;  //DIV
  bus.mmio[0xff05] = this;  //TIMA
  bus.mmio[0xff06] = this;  //TMA
  bus.mmio[0xff07] = this;  //TAC
  bus.mmio[0xff0f] = this;  //IF
  bus.mmio[0xffff] = this;  //IE

  if(Model::GameBoyColor()) {
  bus.mmio[0xff4d] = this;  //KEY1
  bus.mmio[0xff51] = this;  //HDMA1
  bus.mmio[0xff52] = this;  //HDMA2
  bus.mmio[0xff53] = this;  //HDMA3
  bus.mmio[0xff54] = this;  //HDMA4
  bus.mmio[0xff55] = this;  //HDMA5
  bus.mmio[0xff56] = this;  //RP
  bus.mmio[0xff6c] = this;  //???
  bus.mmio[0xff70] = this;  //SVBK
  bus.mmio[0xff72] = this;  //???
  bus.mmio[0xff73] = this;  //???
  bus.mmio[0xff74] = this;  //???
  bus.mmio[0xff75] = this;  //???
  bus.mmio[0xff76] = this;  //???
  bus.mmio[0xff77] = this;  //???
  }

  for(auto& n : wram) n = 0x00;
  for(auto& n : hram) n = 0x00;

  status = {};
}

}
