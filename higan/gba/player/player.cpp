#include <gba/gba.hpp>

namespace GameBoyAdvance {

//Game Boy Player emulation

#include "serialization.cpp"
Player player;

auto Player::power() -> void {
  status.enable = false;
  status.rumble = false;

  status.logoDetected = false;
  status.logoCounter = 0;

  status.packet = 0;
  status.send = 0;
  status.recv = 0;
}

auto Player::frame() -> void {
  uint32 hash = Hash::CRC32(ppu.output, 240 * 160 * sizeof(uint32)).value();
  status.logoDetected = (hash == 0x7776eb55);

  if(status.logoDetected) {
    status.enable = true;
    status.logoCounter = (status.logoCounter + 1) % 3;
    status.packet = 0;
  }

  if(!status.enable) return;

  //todo: verify which settings are actually required
  //values were taken from observing GBP-compatible games
  if(!cpu.regs.joybus.settings.sc
  && !cpu.regs.joybus.settings.sd
  && !cpu.regs.joybus.settings.si
  && !cpu.regs.joybus.settings.so
  && !cpu.regs.joybus.settings.scmode
  && !cpu.regs.joybus.settings.sdmode
  && !cpu.regs.joybus.settings.simode
  && !cpu.regs.joybus.settings.somode
  && !cpu.regs.joybus.settings.irqenable
  && !cpu.regs.joybus.settings.mode
  && !cpu.regs.serial.control.shiftclockselect
  && !cpu.regs.serial.control.shiftclockfrequency
  && !cpu.regs.serial.control.transferenablereceive
  &&  cpu.regs.serial.control.transferenablesend
  &&  cpu.regs.serial.control.startbit
  &&  cpu.regs.serial.control.transferlength
  &&  cpu.regs.serial.control.irqenable
  ) {
    status.packet = (status.packet + 1) % 17;
    switch(status.packet) {
    case  0: status.send = 0x0000494e; break;
    case  1: status.send = 0xb6b1494e; break;
    case  2: status.send = 0xb6b1494e; break;
    case  3: status.send = 0xb6b1544e; break;
    case  4: status.send = 0xabb1544e; break;
    case  5: status.send = 0xabb14e45; break;
    case  6: status.send = 0xb1ba4e45; break;
    case  7: status.send = 0xb1ba4f44; break;
    case  8: status.send = 0xb0bb4f44; break;
    case  9: status.send = 0xb0bb8002; break;
    case 10: status.send = 0x10000010; break;
    case 11: status.send = 0x20000013; break;
    case 12: status.send = 0x30000003; break;
    case 13: status.send = 0x30000003; break;
    case 14: status.send = 0x30000003; break;
    case 15: status.send = 0x30000003; break;
    case 16: status.send = 0x30000003; break;
    }
    cpu.regs.irq.flag |= CPU::Interrupt::Serial;
  }
}

auto Player::keyinput() -> maybe<uint16> {
  if(status.logoDetected) {
    switch(status.logoCounter) {
    case 0: return {0x03ff};
    case 1: return {0x03ff};
    case 2: return {0x030f};
    }
  }
  return nothing;
}

auto Player::read() -> maybe<uint32> {
  if(status.enable) return status.send;
  return nothing;
}

auto Player::write(uint2 addr, uint8 byte) -> void {
  if(!status.enable) return;

  uint shift = addr << 3;
  status.recv &= ~(255 << shift);
  status.recv |= byte << shift;

  if(addr == 3 && status.packet == 15) {
    status.rumble = (status.recv & 0xff) == 0x26;  //on = 0x26, off = 0x04
    interface->inputRumble(0, 0, 10, status.rumble);
  }
}

}
