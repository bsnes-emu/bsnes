#ifdef NALL_DSP_INTERNAL_HPP

#include <math.h>
#include <vector>
#include <nall/stdint.hpp>

namespace nall {

struct DSP;

struct Resampler {
  Resampler(DSP& dsp) : dsp(dsp) {}
  virtual ~Resampler() {}

  virtual auto setFrequency() -> void = 0;
  virtual auto clear() -> void = 0;
  virtual auto sample() -> void = 0;

  DSP& dsp;
  double frequency = 44100.0;
};

struct DSP {
  enum class ResampleEngine : uint {
    Nearest,
    Linear,
    Cosine,
    Cubic,
    Hermite,
    Average,
    Sinc,
  };

  inline DSP();
  inline ~DSP();

  inline auto setChannels(uint channels) -> void;
  inline auto setPrecision(uint precision) -> void;
  inline auto setFrequency(double frequency) -> void;  //inputFrequency
  inline auto setVolume(double volume) -> void;
  inline auto setBalance(double balance) -> void;

  inline auto setResampler(ResampleEngine resamplingEngine) -> void;
  inline auto setResamplerFrequency(double frequency) -> void;  //outputFrequency

  inline auto sample(int channel[]) -> void;
  inline auto pending() const -> bool;
  inline auto read(int channel[]) -> void;

  inline auto clear() -> void;

protected:
  inline auto write(double channel[]) -> void;
  inline auto adjustVolume() -> void;
  inline auto adjustBalance() -> void;
  inline auto clamp(const uint bits, const int input) -> int;

  struct Settings {
    uint channels;
    uint precision;
    double frequency;
    double volume;
    double balance;

    //internal
    double intensity;
    double intensityInverse;
  } settings;

  Resampler* resampler = nullptr;

  #include "buffer.hpp"
  Buffer buffer;
  Buffer output;

  friend class ResampleNearest;
  friend class ResampleLinear;
  friend class ResampleCosine;
  friend class ResampleCubic;
  friend class ResampleAverage;
  friend class ResampleHermite;
  friend class ResampleSinc;
};

#include "resample/nearest.hpp"
#include "resample/linear.hpp"
#include "resample/cosine.hpp"
#include "resample/cubic.hpp"
#include "resample/hermite.hpp"
#include "resample/average.hpp"
#include "resample/sinc.hpp"
#include "settings.hpp"

DSP::DSP() {
  setResampler(ResampleEngine::Hermite);
  setResamplerFrequency(44100.0);

  setChannels(2);
  setPrecision(16);
  setFrequency(44100.0);
  setVolume(1.0);
  setBalance(0.0);

  clear();
}

DSP::~DSP() {
  if(resampler) delete resampler;
}

auto DSP::sample(int channel[]) -> void {
  for(auto c : range(settings.channels)) {
    buffer.write(c) = (double)channel[c] * settings.intensityInverse;
  }
  buffer.wroffset++;
  resampler->sample();
}

auto DSP::pending() const -> bool {
  return output.rdoffset != output.wroffset;
}

auto DSP::read(int channel[]) -> void {
  adjustVolume();
  adjustBalance();

  for(auto c : range(settings.channels)) {
    channel[c] = clamp(settings.precision, output.read(c) * settings.intensity);
  }
  output.rdoffset++;
}

auto DSP::write(double channel[]) -> void {
  for(auto c : range(settings.channels)) {
    output.write(c) = channel[c];
  }
  output.wroffset++;
}

auto DSP::adjustVolume() -> void {
  for(auto c : range(settings.channels)) {
    output.read(c) *= settings.volume;
  }
}

auto DSP::adjustBalance() -> void {
  if(settings.channels != 2) return;  //TODO: support > 2 channels
  if(settings.balance < 0.0) output.read(1) *= 1.0 + settings.balance;
  if(settings.balance > 0.0) output.read(0) *= 1.0 - settings.balance;
}

auto DSP::clamp(const uint bits, const int x) -> int {
  const int b = 1U << (bits - 1);
  const int m = (1U << (bits - 1)) - 1;
  return (x > m) ? m : (x < -b) ? -b : x;
}

auto DSP::clear() -> void {
  buffer.clear();
  output.clear();
  resampler->clear();
}

}

#endif
