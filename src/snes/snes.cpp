#include "../base.h"

#include "snes_video.cpp"
#include "snes_audio.cpp"
#include "snes_input.cpp"

void SNES::run() {
  if(apusync.cycles < 0) {
    r_cpu->run();
    apusync.cycles += apusync.apu_multbl[r_cpu->cycles_executed()];
  } else {
    r_apu->run();
  uint32 cycles = r_apu->cycles_executed();
    apusync.dsp += cycles;
    apusync.cycles -= apusync.cpu_multbl[cycles];

  //1024000(SPC700) / 32000(DSP) = 32spc/dsp ticks
  //24576000(Sound clock crystal) / 32000(DSP) = 768crystal/dsp ticks
    while(apusync.dsp >= 768) {
      apusync.dsp -= 768;
      audio_update(r_dsp->run());
    }
  }
}

void SNES::runtoframe() {
  while(r_ppu->status.frame_executed == false) {
    SNES::run();
  }
  r_ppu->status.frame_executed = false;
}

void SNES::init() {
  srtc = new SRTC();
  sdd1 = new SDD1();
  c4   = new C4();

  srtc->init();
  sdd1->init();
  c4->init();

  video_init();
  audio_init();
}

void SNES::term() {
  audio_term();
}

void SNES::power() {
  r_cpu->power();
  r_apu->power();
  r_dsp->power();
  r_ppu->power();
  r_mem->power();

  if(cartridge.cart.srtc)srtc->power();
  if(cartridge.cart.sdd1)sdd1->power();
  if(cartridge.cart.c4)  c4->power();

int i;
  r_mem->flush_mmio_mappers();
  for(i=0x2100;i<=0x213f;i++)r_mem->set_mmio_mapper(i, r_ppu->mmio);
  for(i=0x2140;i<=0x217f;i++)r_mem->set_mmio_mapper(i, r_cpu->mmio);
  for(i=0x2180;i<=0x2183;i++)r_mem->set_mmio_mapper(i, r_cpu->mmio);
//input
  r_mem->set_mmio_mapper(0x4016, r_cpu->mmio);
  r_mem->set_mmio_mapper(0x4017, r_cpu->mmio);
  for(i=0x4200;i<=0x421f;i++)r_mem->set_mmio_mapper(i, r_cpu->mmio);
  for(i=0x4300;i<=0x437f;i++)r_mem->set_mmio_mapper(i, r_cpu->mmio);

  if(cartridge.cart.srtc)srtc->enable();
  if(cartridge.cart.sdd1)sdd1->enable();
  if(cartridge.cart.c4)  c4->enable();

  video_update();
}

void SNES::reset() {
  apusync.cycles = -apusync.cpu_multbl[32];

  r_cpu->reset();
  r_apu->reset();
  r_dsp->reset();
  r_ppu->reset();
  r_mem->reset();

  if(cartridge.cart.srtc)srtc->reset();
  if(cartridge.cart.sdd1)sdd1->reset();
  if(cartridge.cart.c4)  c4->reset();

  video_update();
}

void SNES::frame() {
  video_update();
}

void SNES::scanline() {
  video_scanline();
}

/****************
 *** PAL/NTSC ***
 ****************/

void SNES::set_region(uint8 new_region) {
  if(new_region == NTSC) {
    snes_region = NTSC;
  } else if(new_region == PAL) {
    snes_region = PAL;
  } else {
    alert("Unsupported region : %0.2x", new_region);
  }

  update_timing();
}

uint8 SNES::region() { return snes_region; }

/**************
 *** Timing ***
 **************/

void SNES::update_timing() {
  apusync.cycles = 0;
  if(snes_region == NTSC) {
    apusync.cpu_freq = 21477272 >> 3;
  } else if(snes_region == PAL) {
    apusync.cpu_freq = 21281370 >> 3;
  }
  apusync.apu_freq = 24576000 >> 3;

int i;
  for(i=0;i<1024;i++) {
    apusync.cpu_multbl[i] = i * apusync.cpu_freq;
    apusync.apu_multbl[i] = i * apusync.apu_freq;
  }
}

/***************************
 *** Debugging functions ***
 ***************************/

void SNES::notify(uint32 message, uint32 param1, uint32 param2) {}

void SNES::debugger_enable() {
  is_debugger_enabled = true;
}

void SNES::debugger_disable() {
  is_debugger_enabled = false;
}

bool SNES::debugger_enabled() {
  return is_debugger_enabled;
}

SNES::SNES() {
  is_debugger_enabled = false;

  snes_region = NTSC;
  update_timing();
}
