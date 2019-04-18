#include <sfc/sfc.hpp>

namespace SuperFamicom {

DSP dsp;

#include "serialization.cpp"
#include "SPC_DSP.cpp"

void DSP::main() {
  if(!configuration.hacks.dsp.fast) {
    spc_dsp.run(1);
    clock += 2;
  } else {
    spc_dsp.run(32);
    clock += 2 * 32;
  }

  signed count = spc_dsp.sample_count();
  if(count > 0) {
    for(unsigned n = 0; n < count; n += 2) {
      stream->sample(samplebuffer[n + 0] / 32767.0, samplebuffer[n + 1] / 32767.0);
    }
    spc_dsp.set_output(samplebuffer, 8192);
  }
}

uint8 DSP::read(uint8 addr) {
  return spc_dsp.read(addr);
}

void DSP::write(uint8 addr, uint8 data) {
  spc_dsp.write(addr, data);
}

bool DSP::load() {
  return true;
}

void DSP::power(bool reset) {
  clock = 0;
  stream = Emulator::audio.createStream(2, system.apuFrequency() / 768.0);

  if(!reset) {
    spc_dsp.init(apuram);
    spc_dsp.reset();
    spc_dsp.set_output(samplebuffer, 8192);
  } else {
    spc_dsp.soft_reset();
    spc_dsp.set_output(samplebuffer, 8192);
  }
}

bool DSP::mute() {
  return false;
}

}
