#pragma once

#include <nall/queue.hpp>
#include <nall/dsp/dsp.hpp>

namespace nall { namespace DSP { namespace Resampler {

struct Cubic {
  inline auto reset(real inputFrequency, real outputFrequency, uint queueSize = 0) -> void;
  inline auto setInputFrequency(real inputFrequency) -> void;
  inline auto pending() const -> bool;
  inline auto read() -> real;
  inline auto write(real sample) -> void;

private:
  real inputFrequency;
  real outputFrequency;

  real ratio;
  real fraction;
  real history[4];
  queue<real> samples;
};

auto Cubic::reset(real inputFrequency, real outputFrequency, uint queueSize) -> void {
  this->inputFrequency = inputFrequency;
  this->outputFrequency = outputFrequency;
  if(!queueSize) queueSize = outputFrequency * 0.02;  //20ms

  ratio = inputFrequency / outputFrequency;
  fraction = 0.0;
  for(auto& sample: history) sample = 0.0;
  samples.resize(queueSize);
}

auto Cubic::setInputFrequency(real inputFrequency) -> void {
  this->inputFrequency = inputFrequency;
  ratio = inputFrequency / outputFrequency;
}

auto Cubic::pending() const -> bool {
  return samples.pending();
}

auto Cubic::read() -> real {
  return samples.read();
}

auto Cubic::write(real sample) -> void {
  auto& mu = fraction;
  auto& s = history;

  s[0] = s[1];
  s[1] = s[2];
  s[2] = s[3];
  s[3] = sample;

  while(mu <= 1.0) {
    real A = s[3] - s[2] - s[0] + s[1];
    real B = s[0] - s[1] - A;
    real C = s[2] - s[0];
    real D = s[1];

    samples.write(A * mu * mu * mu + B * mu * mu + C * mu + D);
    mu += ratio;
  }

  mu -= 1.0;
}

}}}
