#include <../base.hpp>

#include "interface/interface.hpp"
#if defined(SGB_GAMBATTE)
  #include "interface/gambatte.cpp"
#else
  #include "interface/reference.cpp"
#endif

#define SGB_CPP
namespace SNES {

void SuperGameboy::enter() {
  while(true) {
    unsigned clocks;
    if((mmio.r6003 & 0x80) == 0) {
      clocks = 10;
      system.audio.cop_sample(0, 0);
    } else {
      clocks = gameboy->run();
    }
    scheduler.addclocks_cop(clocks);
    scheduler.sync_copcpu();
  }
}

uint8_t SuperGameboy::read(unsigned addr) {
  addr &= 0xffff;

  //DMG lyCounter
  if(addr == 0x6000) {
    return gameboy->lyCounter();
  }

  //command ready port
  if(addr == 0x6002) {
    bool data = packet.size() > 0;
    if(data) {
      mmio.r7000 = packet[0];
      unsigned size = packet.size() - 1;
      for(unsigned i = 0; i < size; i++) packet[i] = packet[i + 1];
      packet.resize(size);
    }
    return data;
  }

  //command port
  if((addr & 0xfff0) == 0x7000) {
    return mmio.r7000[addr & 15];
  }

  //screen data port
  if(addr == 0x7800) {
    uint8_t data = gameboy->vram[mmio.r7800++];
    if(mmio.r7800 >= (160 / 8) * (144 / 8) * (64 / 4)) mmio.r7800 = 0;
    return data;
  }

  return 0x00;
}

void SuperGameboy::write(unsigned addr, uint8_t data) {
  addr &= 0xffff;

  if(addr == 0x6001) {
    gameboy->updateVideo(mmio.r7800 / 320);
  }

  //control port
  //d7 = Gameboy enable
  //d5 = four-player enable (1=4-player, 0=see d4)
  //d4 = two-player enable  (if: d5=0; 1=2-player, 0=1-player)
  //d0 = ??? (always 1)
  if(addr == 0x6003) {
    if(((mmio.r6003 & 0x80) == 0x00) && ((data & 0x80) == 0x80)) {
      gameboy->reset();
      command_1e();
      mmio.r7800 = 320 * 16;
    }
    mmio.r6003 = data;
    return;
  }

  //joypad ports 1-4
  if((addr & 0x600c) == 0x6004) {
    mmio.joypad[addr & 3] = data;
    return;
  }
}

void SuperGameboy::init() {}
void SuperGameboy::enable() {}

void SuperGameboy::power() {
  multiplier = (system.region() == System::NTSC ? config.cpu.ntsc_clock_rate : config.cpu.pal_clock_rate);
  gameboy->power();
  reset();
}

void SuperGameboy::reset() {
  gameboy->reset();
  packet.reset();
  counter = 0;

  bus.map(Bus::MapDirect, 0x00, 0x3f, 0x6000, 0x7fff, *this);
  bus.map(Bus::MapDirect, 0x80, 0xbf, 0x6000, 0x7fff, *this);

  mmio.r6000 = 0;
  mmio.r6003 = 0;

  //$6004-$6007
  mmio.joypadid = 0;
  for(unsigned i = 0; i < 4; i++) mmio.joypad[i] = 0xff;

  for(unsigned i = 0; i < 16; i++) mmio.r7000[i] = 0;
  mmio.r7800 = 0;
}

void SuperGameboy::unload() {
  gameboy->unload();
}

//

void SuperGameboy::command_1e() {
  for(unsigned i = 0; i < 6; i++) {
    Packet p;
    p[0] = (0x1e << 3) + 1;
    p[1] = 0;
    for(unsigned n = 2; n < 16; n++) {
      p[n] = memory::dmgrom.read(0x0104 + (i * 14) + (n - 2));
    }
    packet.add(p);
  }
}

//

SuperGameboy::SuperGameboy() {
  gameboy = new GameboyGambatte;
}

SuperGameboy::~SuperGameboy() {
  delete gameboy;
}

};
