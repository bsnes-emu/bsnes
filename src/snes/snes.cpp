#include "../base.h"
#define SNES_CPP

SNES     snes;
BSXBase  bsxbase;
BSXCart  bsxcart;
BSXFlash bsxflash;
SRTC     srtc;
SDD1     sdd1;
Cx4      cx4;
DSP1     dsp1;
DSP2     dsp2;
DSP3     dsp3;
DSP4     dsp4;
OBC1     obc1;
ST010    st010;

#include "scheduler/scheduler.cpp"
#include "tracer/tracer.cpp"

#include "video/video.cpp"
#include "audio/audio.cpp"
#include "input/input.cpp"

void SNES::run() {}

void SNES::runtoframe() {
  scheduler.enter();
}

void SNES::init() {
  bsxbase.init();
  bsxcart.init();
  bsxflash.init();
  srtc.init();
  sdd1.init();
  cx4.init();
  dsp1.init();
  dsp2.init();
  dsp3.init();
  dsp4.init();
  obc1.init();
  st010.init();

  video.init();
  audio.init();
  input.init();
  snesinterface.init();
}

void SNES::term() {
  audio.term();
  snesinterface.term();
}

void SNES::power() {
  scheduler.init();

  cpu.power();
  smp.power();
  dsp.power();
  ppu.power();
  bus.power();

  if(cartridge.info.bsxbase)  bsxbase.power();
  if(cartridge.info.bsxcart)  bsxcart.power();
  if(cartridge.info.bsxflash) bsxflash.power();
  if(cartridge.info.srtc)     srtc.power();
  if(cartridge.info.sdd1)     sdd1.power();
  if(cartridge.info.cx4)      cx4.power();
  if(cartridge.info.dsp1)     dsp1.power();
  if(cartridge.info.dsp2)     dsp2.power();
  if(cartridge.info.dsp3)     dsp3.power();
  if(cartridge.info.dsp4)     dsp4.power();
  if(cartridge.info.obc1)     obc1.power();
  if(cartridge.info.st010)    st010.power();

  for(uint16_t i = 0x2100; i <= 0x213f; i++) memory::mmio.map(i, ppu);
  for(uint16_t i = 0x2140; i <= 0x217f; i++) memory::mmio.map(i, cpu);
  for(uint16_t i = 0x2180; i <= 0x2183; i++) memory::mmio.map(i, cpu);
  for(uint16_t i = 0x4016; i <= 0x4017; i++) memory::mmio.map(i, cpu);
  for(uint16_t i = 0x4200; i <= 0x421f; i++) memory::mmio.map(i, cpu);
  for(uint16_t i = 0x4300; i <= 0x437f; i++) memory::mmio.map(i, cpu);

  if(cartridge.info.bsxbase)  bsxbase.enable();
  if(cartridge.info.bsxcart)  bsxcart.enable();
  if(cartridge.info.bsxflash) bsxflash.enable();
  if(cartridge.info.srtc)     srtc.enable();
  if(cartridge.info.sdd1)     sdd1.enable();
  if(cartridge.info.cx4)      cx4.enable();
  if(cartridge.info.dsp1)     dsp1.enable();
  if(cartridge.info.dsp2)     dsp2.enable();
  if(cartridge.info.dsp3)     dsp3.enable();
  if(cartridge.info.dsp4)     dsp4.enable();
  if(cartridge.info.obc1)     obc1.enable();
  if(cartridge.info.st010)    st010.enable();

  video.update();
}

void SNES::reset() {
  scheduler.init();

  cpu.reset();
  smp.reset();
  dsp.reset();
  ppu.reset();
  bus.reset();

  if(cartridge.info.bsxbase)  bsxbase.reset();
  if(cartridge.info.bsxcart)  bsxcart.reset();
  if(cartridge.info.bsxflash) bsxflash.reset();
  if(cartridge.info.srtc)     srtc.reset();
  if(cartridge.info.sdd1)     sdd1.reset();
  if(cartridge.info.cx4)      cx4.reset();
  if(cartridge.info.dsp1)     dsp1.reset();
  if(cartridge.info.dsp2)     dsp2.reset();
  if(cartridge.info.dsp3)     dsp3.reset();
  if(cartridge.info.dsp4)     dsp4.reset();
  if(cartridge.info.obc1)     obc1.reset();
  if(cartridge.info.st010)    st010.reset();

  video.update();
}

void SNES::scanline() {
  video.scanline();

  if(cpu.vcounter() == 241) {
    video.update();
    scheduler.exit();
  }
}

void SNES::frame() {}
void SNES::set_region(Region region) { snes_region = region; }
SNES::Region SNES::region() { return snes_region; }
SNES::SNES() {}
