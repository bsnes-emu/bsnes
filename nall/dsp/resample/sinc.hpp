#pragma once

#include "lib/sinc.hpp"

struct ResampleSinc : Resampler {
  inline ResampleSinc(DSP& dsp);
  inline ~ResampleSinc();

  inline auto setFrequency() -> void;
  inline auto clear() -> void;
  inline auto sample() -> void;

private:
  inline void remakeSinc();
  SincResample* sincResampler[8] = {0};
};

ResampleSinc::ResampleSinc(DSP& dsp) : Resampler(dsp) {
  for(auto n : range(8)) {
    sincResampler[n] = nullptr;
  }
}

ResampleSinc::~ResampleSinc() {
  for(auto n : range(8)) {
    if(sincResampler[n]) delete sincResampler[n];
  }
}

auto ResampleSinc::setFrequency() -> void {
  remakeSinc();
}

auto ResampleSinc::clear() -> void {
  remakeSinc();
}

auto ResampleSinc::sample() -> void {
  for(auto c : range(dsp.settings.channels)) {
    sincResampler[c]->write(dsp.buffer.read(c));
  }

  if(sincResampler[0]->output_avail()) {
    do {
      for(auto c : range(dsp.settings.channels)) {
        dsp.output.write(c) = sincResampler[c]->read();
      }
      dsp.output.wroffset++;
    } while(sincResampler[0]->output_avail());
  }

  dsp.buffer.rdoffset++;
}

auto ResampleSinc::remakeSinc() -> void {
  assert(dsp.settings.channels < 8);

  for(auto c : range(dsp.settings.channels)) {
    if(sincResampler[c]) delete sincResampler[c];
    sincResampler[c] = new SincResample(dsp.settings.frequency, frequency, 0.85, SincResample::QUALITY_HIGH);
  }
}
