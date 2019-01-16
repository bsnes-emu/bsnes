#pragma once

#include <nall/queue.hpp>
#include <nall/dsp/dsp.hpp>

namespace nall::DSP::Resampler {

struct Cubic {
  inline auto reset(double inputFrequency, double outputFrequency = 0, uint queueSize = 0) -> void;
  inline auto setInputFrequency(double inputFrequency) -> void;
  inline auto pending() const -> bool;
  inline auto read() -> double;
  inline auto write(double sample) -> void;

private:
  double inputFrequency;
  double outputFrequency;

  double ratio;
  double fraction;
  double history[4];
  queue<double> samples;
};

auto Cubic::reset(double inputFrequency, double outputFrequency, uint queueSize) -> void {
  this->inputFrequency = inputFrequency;
  this->outputFrequency = outputFrequency ? outputFrequency : this->inputFrequency;

  ratio = inputFrequency / outputFrequency;
  fraction = 0.0;
  for(auto& sample : history) sample = 0.0;
  samples.resize(queueSize ? queueSize : this->outputFrequency * 0.02);  //default to 20ms max queue size
}

auto Cubic::setInputFrequency(double inputFrequency) -> void {
  this->inputFrequency = inputFrequency;
  ratio = inputFrequency / outputFrequency;
}

auto Cubic::pending() const -> bool {
  return samples.pending();
}

auto Cubic::read() -> double {
  return samples.read();
}

auto Cubic::write(double sample) -> void {
  auto& mu = fraction;
  auto& s = history;

  s[0] = s[1];
  s[1] = s[2];
  s[2] = s[3];
  s[3] = sample;

  while(mu <= 1.0) {
    double A = s[3] - s[2] - s[0] + s[1];
    double B = s[0] - s[1] - A;
    double C = s[2] - s[0];
    double D = s[1];

    samples.write(A * mu * mu * mu + B * mu * mu + C * mu + D);
    mu += ratio;
  }

  mu -= 1.0;
}

}
