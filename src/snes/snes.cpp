#include "../base.h"

SNES snes;

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
  srtc = new SRTC();
  sdd1 = new SDD1();
  c4   = new C4();
  dsp1 = new DSP1();
  dsp2 = new DSP2();
  obc1 = new OBC1();

  srtc->init();
  sdd1->init();
  c4->init();
  dsp1->init();
  dsp2->init();
  obc1->init();

  video_init();
  audio_init();
  input_init();
}

void SNES::term() {
  audio_term();
}

void SNES::power() {
  scheduler.init();

  r_cpu->power();
  r_smp->power();
  r_dsp->power();
  r_ppu->power();
  r_mem->power();

  if(cartridge.info.srtc)srtc->power();
  if(cartridge.info.sdd1)sdd1->power();
  if(cartridge.info.c4)  c4->power();
  if(cartridge.info.dsp1)dsp1->power();
  if(cartridge.info.dsp2)dsp2->power();
  if(cartridge.info.obc1)obc1->power();

  r_mem->flush_mmio_mappers();
  for(int i = 0x2100; i <= 0x213f; i++)r_mem->set_mmio_mapper(i, r_ppu);
  for(int i = 0x2140; i <= 0x217f; i++)r_mem->set_mmio_mapper(i, r_cpu);
  for(int i = 0x2180; i <= 0x2183; i++)r_mem->set_mmio_mapper(i, r_cpu);
  for(int i = 0x4016; i <= 0x4017; i++)r_mem->set_mmio_mapper(i, r_cpu);
  for(int i = 0x4200; i <= 0x421f; i++)r_mem->set_mmio_mapper(i, r_cpu);
  for(int i = 0x4300; i <= 0x437f; i++)r_mem->set_mmio_mapper(i, r_cpu);

  if(cartridge.info.srtc)srtc->enable();
  if(cartridge.info.sdd1)sdd1->enable();
  if(cartridge.info.c4)  c4->enable();
  if(cartridge.info.dsp1)dsp1->enable();
  if(cartridge.info.dsp2)dsp2->enable();
  if(cartridge.info.obc1)obc1->enable();

  video_update();
}

void SNES::reset() {
  scheduler.init();

  r_cpu->reset();
  r_smp->reset();
  r_dsp->reset();
  r_ppu->reset();
  r_mem->reset();

  if(cartridge.info.srtc)srtc->reset();
  if(cartridge.info.sdd1)sdd1->reset();
  if(cartridge.info.c4)  c4->reset();
  if(cartridge.info.dsp1)dsp1->reset();
  if(cartridge.info.dsp2)dsp2->reset();
  if(cartridge.info.obc1)obc1->reset();

  video_update();
}

void SNES::scanline() {
  video_scanline();

//draw before the start of the next frame, to make the
//video output seem more responsive to controller input
  if(r_cpu->vcounter() == 241) {
    video_update();
    scheduler.exit();
  }
}

void SNES::frame() {}

/*****
 * PAL/NTSC
 *****/

void SNES::set_region(uint8 new_region) {
  if(new_region == NTSC) {
    snes_region = NTSC;
  } else if(new_region == PAL) {
    snes_region = PAL;
  } else {
    alert("Unsupported region : %0.2x", new_region);
  }
}

uint8 SNES::region() { return snes_region; }

SNES::SNES() {}
