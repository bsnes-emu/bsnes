#include "../base.h"

#include "snes_video.cpp"
#include "snes_audio.cpp"
#include "snes_input.cpp"

void SNES::run() {
uint32 cycles, r;
  if(apusync.cycles < 0) {
    cpu->run();
    apusync.cycles += apusync.apu_multbl[cpu->cycles_executed()];
  } else {
    apu->run();
    cycles = apu->cycles_executed();
    apusync.dsp += cycles;
    apusync.cycles -= apusync.cpu_multbl[cycles];

  //1024000(SPC700) / 32000(DSP) = 32spc/dsp ticks
  //24576000(Sound clock crystal) / 32000(DSP) = 768crystal/dsp ticks
    while(apusync.dsp >= 768) {
      apusync.dsp -= 768;
      audio_update(dsp->run());
    }
  }
}

void SNES::init() {
  srtc = new SRTC();
  sdd1 = new SDD1();

  srtc->init();
  sdd1->init();

  video_init();
  audio_init();
}

void SNES::term() {
  audio_term();
}

void SNES::power() {
  cpu->power();
  apu->power();
  dsp->power();
  ppu->power();
  mem_bus->power();

  srtc->power();
  sdd1->power();

int i;
  mem_bus->flush_mmio_mappers();
  for(i=0x2100;i<=0x213f;i++)mem_bus->set_mmio_mapper(i, ppu->mmio);
  for(i=0x2140;i<=0x217f;i++)mem_bus->set_mmio_mapper(i, cpu->mmio);
  for(i=0x2180;i<=0x2183;i++)mem_bus->set_mmio_mapper(i, cpu->mmio);
//input
  mem_bus->set_mmio_mapper(0x4016, cpu->mmio);
  mem_bus->set_mmio_mapper(0x4017, cpu->mmio);
  for(i=0x4200;i<=0x421f;i++)mem_bus->set_mmio_mapper(i, cpu->mmio);
  for(i=0x4300;i<=0x437f;i++)mem_bus->set_mmio_mapper(i, cpu->mmio);

  srtc->enable();
  sdd1->enable();

  memset(video.data,     0, 512 * 448 * sizeof(uint32));
  memset(video.ppu_data, 0, 512 * 480 * sizeof(uint16));
  video_update();
}

void SNES::reset() {
  apusync.cycles = -apusync.cpu_multbl[32];

  cpu->reset();
  apu->reset();
  dsp->reset();
  ppu->reset();
  mem_bus->reset();

  srtc->reset();
  sdd1->reset();

  memset(video.data,     0, 512 * 448 * sizeof(uint32));
  memset(video.ppu_data, 0, 512 * 480 * sizeof(uint16));
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

  dsp_buffer.data = 0;
}
