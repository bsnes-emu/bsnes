#include <../base.hpp>

#define SNES_CPP
namespace SNES {

System system;
Config config;

BUSCORE bus;
CPUCORE cpu;
SMPCORE smp;
DSPCORE dsp;
PPUCORE ppu;

SuperGameboy sgb;
SA1          sa1;
SA1Bus       sa1bus;
BSXBase      bsxbase;
BSXCart      bsxcart;
BSXFlash     bsxflash;
SRTC         srtc;
SDD1         sdd1;
SPC7110      spc7110;
Cx4          cx4;
DSP1         dsp1;
DSP2         dsp2;
DSP3         dsp3;
DSP4         dsp4;
OBC1         obc1;
ST010        st010;

#include "scheduler/scheduler.cpp"
#include "tracer/tracer.cpp"
#include "config/config.cpp"

#include "video/video.cpp"
#include "audio/audio.cpp"
#include "input/input.cpp"

void System::coprocessor_enter() {
  if(cartridge.mode() == Cartridge::ModeSuperGameboy) sgb.enter();
  if(cartridge.has_sa1()) sa1.enter();

  //no active co-processor
  while(true) {
    scheduler.addclocks_cop(64 * 1024 * 1024);
    scheduler.sync_copcpu();
  }
}

void System::run() {
}

void System::runtoframe() {
  scheduler.enter();
}

void System::init(Interface *interface_) {
  interface = interface_;

  sgb.init();
  sa1.init();
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
  interface->init();
}

void System::term() {
  interface->term();
}

void System::power() {
  snes_region = max(0, min(2, config.region));
  snes_expansion = max(0, min(1, config.expansion_port));

  if(snes_region == Autodetect) {
    snes_region = (cartridge.region() == Cartridge::NTSC ? NTSC : PAL);
  }

  scheduler.init();
  unsigned freq = (snes_region == NTSC ? config.smp.ntsc_clock_rate : config.smp.pal_clock_rate);
  audio.set_dsp_frequency(freq / 768);
  audio.set_cop_frequency(0);  //disable bus audio by default

  ppu.PPUcounter::reset();
  cpu.power();
  smp.power();
  dsp.power();
  ppu.power();
  bus.power();

  if(expansion() == ExpansionBSX) bsxbase.power();
  if(cartridge.mode() == Cartridge::ModeBsx) bsxcart.power();
  if(cartridge.bsx_flash_loaded()) bsxflash.power();
  if(cartridge.mode() == Cartridge::ModeSuperGameboy) sgb.power();

  if(cartridge.has_sa1())     sa1.power();
  if(cartridge.has_srtc())    srtc.power();
  if(cartridge.has_sdd1())    sdd1.power();
  if(cartridge.has_spc7110()) spc7110.power();
  if(cartridge.has_cx4())     cx4.power();
  if(cartridge.has_dsp1())    dsp1.power();
  if(cartridge.has_dsp2())    dsp2.power();
  if(cartridge.has_dsp3())    dsp3.power();
  if(cartridge.has_dsp4())    dsp4.power();
  if(cartridge.has_obc1())    obc1.power();
  if(cartridge.has_st010())   st010.power();

  for(unsigned i = 0x2100; i <= 0x213f; i++) memory::mmio.map(i, ppu);
  for(unsigned i = 0x2140; i <= 0x217f; i++) memory::mmio.map(i, cpu);
  for(unsigned i = 0x2180; i <= 0x2183; i++) memory::mmio.map(i, cpu);
  for(unsigned i = 0x4016; i <= 0x4017; i++) memory::mmio.map(i, cpu);
  for(unsigned i = 0x4200; i <= 0x421f; i++) memory::mmio.map(i, cpu);
  for(unsigned i = 0x4300; i <= 0x437f; i++) memory::mmio.map(i, cpu);

  if(expansion() == ExpansionBSX) bsxbase.enable();
  if(cartridge.mode() == Cartridge::ModeBsx) bsxcart.enable();
  if(cartridge.bsx_flash_loaded()) bsxflash.enable();
  if(cartridge.mode() == Cartridge::ModeSuperGameboy) sgb.enable();

  if(cartridge.has_sa1())     sa1.enable();
  if(cartridge.has_srtc())    srtc.enable();
  if(cartridge.has_sdd1())    sdd1.enable();
  if(cartridge.has_spc7110()) spc7110.enable();
  if(cartridge.has_cx4())     cx4.enable();
  if(cartridge.has_dsp1())    dsp1.enable();
  if(cartridge.has_dsp2())    dsp2.enable();
  if(cartridge.has_dsp3())    dsp3.enable();
  if(cartridge.has_dsp4())    dsp4.enable();
  if(cartridge.has_obc1())    obc1.enable();
  if(cartridge.has_st010())   st010.enable();

  input.port_set_device(0, config.controller_port1);
  input.port_set_device(1, config.controller_port2);
  input.update();
  video.update();
}

void System::reset() {
  scheduler.init();
  unsigned freq = (snes_region == NTSC ? config.smp.ntsc_clock_rate : config.smp.pal_clock_rate);
  audio.set_dsp_frequency(freq / 768);
  audio.set_cop_frequency(0);  //disable bus audio by default

  ppu.PPUcounter::reset();
  cpu.reset();
  smp.reset();
  dsp.reset();
  ppu.reset();
  bus.reset();

  if(expansion() == ExpansionBSX) bsxbase.reset();
  if(cartridge.mode() == Cartridge::ModeBsx) bsxcart.reset();
  if(cartridge.bsx_flash_loaded()) bsxflash.reset();
  if(cartridge.mode() == Cartridge::ModeSuperGameboy) sgb.reset();

  if(cartridge.has_sa1())     sa1.reset();
  if(cartridge.has_srtc())    srtc.reset();
  if(cartridge.has_sdd1())    sdd1.reset();
  if(cartridge.has_spc7110()) spc7110.reset();
  if(cartridge.has_cx4())     cx4.reset();
  if(cartridge.has_dsp1())    dsp1.reset();
  if(cartridge.has_dsp2())    dsp2.reset();
  if(cartridge.has_dsp3())    dsp3.reset();
  if(cartridge.has_dsp4())    dsp4.reset();
  if(cartridge.has_obc1())    obc1.reset();
  if(cartridge.has_st010())   st010.reset();

  input.port_set_device(0, config.controller_port1);
  input.port_set_device(1, config.controller_port2);
  input.update();
  video.update();
}

void System::scanline() {
  video.scanline();

  if(ppu.vcounter() == 241) {
    input.update();
    video.update();
    scheduler.exit();
  }
}

void System::frame() {
}

System::Region System::region() const {
  return (System::Region)snes_region;
}

System::ExpansionPortDevice System::expansion() const {
  return (System::ExpansionPortDevice)snes_expansion;
}

System::System() : snes_region(NTSC), snes_expansion(ExpansionNone) {
}

};
