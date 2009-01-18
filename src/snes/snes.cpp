#include <../base.hpp>
#include <../chip/chip.hpp>
#include <../cart/cart.hpp>
#define SNES_CPP

SNES     snes;
BUSCORE  bus;
CPUCORE  cpu;
SMPCORE  smp;
DSPCORE  dsp;
PPUCORE  ppu;

BSXBase  bsxbase;
BSXCart  bsxcart;
BSXFlash bsxflash;
SRTC     srtc;
SDD1     sdd1;
SPC7110  spc7110;
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

void SNES::run() {
}

void SNES::runtoframe() {
  scheduler.enter();
}

void SNES::init() {
  bsxbase.init();
  bsxcart.init();
  bsxflash.init();
  srtc.init();
  sdd1.init();
  spc7110.init();
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
  snesinterface.term();
}

void SNES::power() {
  snes_region = max(0, min(2, snes.config.region));
  snes_expansion = max(0, min(1, snes.config.expansion_port));

  if(snes_region == Autodetect) {
    snes_region = cartridge.region() == Cartridge::NTSC ? NTSC : PAL;
  }

  scheduler.init();

  ppu.PPUcounter::reset();
  cpu.power();
  smp.power();
  dsp.power();
  ppu.power();
  bus.power();

  if(expansion() == ExpansionBSX) bsxbase.power();

  if(cartridge.info.bsxcart)  bsxcart.power();
  if(cartridge.info.bsxflash) bsxflash.power();
  if(cartridge.info.srtc)     srtc.power();
  if(cartridge.info.sdd1)     sdd1.power();
  if(cartridge.info.spc7110)  spc7110.power();
  if(cartridge.info.cx4)      cx4.power();
  if(cartridge.info.dsp1)     dsp1.power();
  if(cartridge.info.dsp2)     dsp2.power();
  if(cartridge.info.dsp3)     dsp3.power();
  if(cartridge.info.dsp4)     dsp4.power();
  if(cartridge.info.obc1)     obc1.power();
  if(cartridge.info.st010)    st010.power();

  for(unsigned i = 0x2100; i <= 0x213f; i++) memory::mmio.map(i, ppu);
  for(unsigned i = 0x2140; i <= 0x217f; i++) memory::mmio.map(i, cpu);
  for(unsigned i = 0x2180; i <= 0x2183; i++) memory::mmio.map(i, cpu);
  for(unsigned i = 0x4016; i <= 0x4017; i++) memory::mmio.map(i, cpu);
  for(unsigned i = 0x4200; i <= 0x421f; i++) memory::mmio.map(i, cpu);
  for(unsigned i = 0x4300; i <= 0x437f; i++) memory::mmio.map(i, cpu);

  if(expansion() == ExpansionBSX) bsxbase.enable();

  if(cartridge.info.bsxcart)  bsxcart.enable();
  if(cartridge.info.bsxflash) bsxflash.enable();
  if(cartridge.info.srtc)     srtc.enable();
  if(cartridge.info.sdd1)     sdd1.enable();
  if(cartridge.info.spc7110)  spc7110.enable();
  if(cartridge.info.cx4)      cx4.enable();
  if(cartridge.info.dsp1)     dsp1.enable();
  if(cartridge.info.dsp2)     dsp2.enable();
  if(cartridge.info.dsp3)     dsp3.enable();
  if(cartridge.info.dsp4)     dsp4.enable();
  if(cartridge.info.obc1)     obc1.enable();
  if(cartridge.info.st010)    st010.enable();

  input.port_set_device(0, snes.config.controller_port1);
  input.port_set_device(1, snes.config.controller_port2);
  input.update();
  video.update();
}

void SNES::reset() {
  scheduler.init();

  ppu.PPUcounter::reset();
  cpu.reset();
  smp.reset();
  dsp.reset();
  ppu.reset();
  bus.reset();

  if(expansion() == ExpansionBSX) bsxbase.reset();

  if(cartridge.info.bsxcart)  bsxcart.reset();
  if(cartridge.info.bsxflash) bsxflash.reset();
  if(cartridge.info.srtc)     srtc.reset();
  if(cartridge.info.sdd1)     sdd1.reset();
  if(cartridge.info.spc7110)  spc7110.reset();
  if(cartridge.info.cx4)      cx4.reset();
  if(cartridge.info.dsp1)     dsp1.reset();
  if(cartridge.info.dsp2)     dsp2.reset();
  if(cartridge.info.dsp3)     dsp3.reset();
  if(cartridge.info.dsp4)     dsp4.reset();
  if(cartridge.info.obc1)     obc1.reset();
  if(cartridge.info.st010)    st010.reset();

  input.port_set_device(0, snes.config.controller_port1);
  input.port_set_device(1, snes.config.controller_port2);
  input.update();
  video.update();
}

void SNES::scanline() {
  video.scanline();

  if(ppu.vcounter() == 241) {
    input.update();
    video.update();
    scheduler.exit();
  }
}

void SNES::frame() {
}

SNES::Region SNES::region() const {
  return (SNES::Region)snes_region;
}

SNES::ExpansionPortDevice SNES::expansion() const {
  return (SNES::ExpansionPortDevice)snes_expansion;
}

SNES::SNES() : snes_region(NTSC), snes_expansion(ExpansionNone) {
  config.controller_port1 = Input::DeviceJoypad;
  config.controller_port2 = Input::DeviceJoypad;
  config.expansion_port   = ExpansionBSX;
  config.region           = Autodetect;

  config.file.autodetect_type    = false;
  config.file.bypass_patch_crc32 = false;

  config.path.base       = "";
  config.path.user       = "";
  config.path.rom        = "";
  config.path.save       = "";
  config.path.patch      = "";
  config.path.cheat      = "";
  config.path.exportdata = "";
  config.path.bsx        = "";
  config.path.st         = "";

  config.cpu.ntsc_clock_rate = 21477272;
  config.cpu.pal_clock_rate  = 21281370;
  config.cpu.alu_mul_delay   = 2;
  config.cpu.alu_div_delay   = 2;
  config.cpu.wram_init_value = 0x55;

  config.smp.ntsc_clock_rate = 32041 * 768;
  config.smp.pal_clock_rate  = 32041 * 768;
}
