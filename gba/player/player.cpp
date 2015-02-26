#include <gba/gba.hpp>

namespace GameBoyAdvance {

//Game Boy Player emulation

#include "serialization.cpp"
Player player;

void Player::power() {
  status.enable = false;
  status.rumble = false;

  status.logoDetected = false;
  status.logoCounter = 0;

  status.packet = 0;
  status.send = 0;
  status.recv = 0;
}

void Player::frame() {
  uint32 hash = Hash::CRC32(ppu.output, 240 * 160 * sizeof(uint32)).value();
  status.logoDetected = (hash == 0x7776eb55);

  if(status.logoDetected) {
    status.enable = true;
    status.logoCounter = (status.logoCounter + 1) % 3;
    status.packet = 0;
  }

  if(status.enable == false) return;

  if(cpu.regs.joybus.settings == 0x0000 && cpu.regs.serial.control == 0x5088) {
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
    cpu.regs.irq.flag.serial = true;
  }
}

maybe<uint16> Player::keyinput() {
  if(status.logoDetected) {
    switch(status.logoCounter) {
    case 0: return 0x03ff;
    case 1: return 0x03ff;
    case 2: return 0x030f;
    }
  }
  return nothing;
}

maybe<uint32> Player::read() {
  if(status.enable) return status.send;
  return nothing;
}

void Player::write(uint8 byte, uint2 addr) {
  if(status.enable == false) return;

  unsigned shift = addr << 3;
  status.recv &= ~(255 << shift);
  status.recv |= byte << shift;

  if(addr == 3 && status.packet == 15) {
    status.rumble = (status.recv & 0xff) == 0x26;  //on = 0x26, off = 0x04
    interface->inputRumble(0, 0, 10, status.rumble);
  }
}

}
