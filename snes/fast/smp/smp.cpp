#include <snes.hpp>

#define SMP_CPP
namespace SNES {

SMP smp;

#include "../snes_spc/dsp.cpp"
#include "../snes_spc/SNES_SPC.cpp"
#include "../snes_spc/SNES_SPC_misc.cpp"
#include "../snes_spc/SNES_SPC_state.cpp"
#include "../snes_spc/spc.cpp"
#include "../snes_spc/SPC_DSP.cpp"
#include "../snes_spc/SPC_Filter.cpp"

#include "serialization.cpp"
#include "iplrom.cpp"

void SMP::step(unsigned clocks) {
  clock += clocks * (uint64)cpu.frequency;
}

void SMP::synchronize_cpu() {
}

void SMP::synchronize_dsp() {
}

uint8 SMP::port_read(uint8 port) {
  return snes_spc.read_port(snes_spc_time, port & 3);
}

void SMP::port_write(uint8 port, uint8 data) {
  snes_spc.write_port(snes_spc_time, port & 3, data);
}

void SMP::run() {
  step(24);
  if(++snes_spc_time >= snes_spc.clock_rate / 60) {
    snes_spc.end_frame(snes_spc_time);
    snes_spc_time = 0;

    signed count = snes_spc.sample_count();
    if(count > 0) {
      for(unsigned n = 0; n < count; n += 2) audio.sample(samplebuffer[n + 0], samplebuffer[n + 1]);
      snes_spc.set_output(samplebuffer, 8192);
    }

  //while(signed count = snes_spc.read_samples(buffer, 8192)) {
  //  for(unsigned n = 0; n < count; n += 2) audio.sample(samplebuffer[n + 0], samplebuffer[n + 1]);
  //}
  }
}

static void Enter() {}

void SMP::power() {
  create(Enter, 24576000);  //system.apu_frequency()
  snes_spc.init();
  snes_spc.init_rom(iplrom);
  snes_spc.reset();
  snes_spc_time = 0;
}

void SMP::reset() {
  snes_spc.soft_reset();
  snes_spc.set_output(samplebuffer, 8192);
  snes_spc_time = 0;
}

SMP::SMP() {
}

SMP::~SMP() {
}

}
