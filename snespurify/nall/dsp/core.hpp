#ifdef NALL_DSP_INTERNAL_HPP

#include <math.h>
#include <nall/stdint.hpp>

namespace nall {

struct DSP {
  enum class Resampler : unsigned {
    Point,
    Linear,
    Cosine,
    Cubic,
    Hermite,
    Average,
  };

  inline void setPrecision(unsigned precision);
  inline void setFrequency(double frequency);  //inputFrequency
  inline void setVolume(double volume);
  inline void setBalance(double balance);

  inline void setResampler(Resampler resampler);
  inline void setResamplerFrequency(double frequency);  //outputFrequency

  inline void sample(signed lchannel, signed rchannel);
  inline bool pending();
  inline void read(signed &lchannel, signed &rchannel);

  inline void clear();
  inline DSP();
  inline ~DSP();

protected:
  struct Settings {
    unsigned precision;
    double frequency;
    double volume;
    double balance;
    //internal
    double intensity;
  } settings;

  struct ResamplerSettings {
    Resampler engine;
    double frequency;
    //internal
    double fraction;
    double step;
  } resampler;

  inline void resamplerRun();
  inline void resamplerWrite(double lchannel, double rchannel);

  inline void resamplePoint();
  inline void resampleLinear();
  inline void resampleCosine();
  inline void resampleCubic();
  inline void resampleHermite();
  inline void resampleAverage();

  #include "buffer.hpp"
  Buffer buffer;
  Buffer output;

  inline void adjustVolume();
  inline void adjustBalance();
  inline signed clamp(const unsigned bits, const signed x);
};

#include "settings.hpp"

void DSP::sample(signed lchannel, signed rchannel) {
  buffer.write(0) = (double)lchannel / settings.intensity;
  buffer.write(1) = (double)rchannel / settings.intensity;
  buffer.wroffset++;
  resamplerRun();
}

bool DSP::pending() {
  return output.rdoffset != output.wroffset;
}

void DSP::read(signed &lchannel, signed &rchannel) {
  adjustVolume();
  adjustBalance();

  lchannel = clamp(settings.precision, output.read(0) * settings.intensity);
  rchannel = clamp(settings.precision, output.read(1) * settings.intensity);
  output.rdoffset++;
}

void DSP::resamplerRun() {
  switch(resampler.engine) {
  case Resampler::Point:   return resamplePoint();
  case Resampler::Linear:  return resampleLinear();
  case Resampler::Cosine:  return resampleCosine();
  case Resampler::Cubic:   return resampleCubic();
  case Resampler::Hermite: return resampleHermite();
  case Resampler::Average: return resampleAverage();
  }
}

void DSP::resamplerWrite(double lchannel, double rchannel) {
  output.write(0) = lchannel;
  output.write(1) = rchannel;
  output.wroffset++;
}

#include "resample/point.hpp"
#include "resample/linear.hpp"
#include "resample/cosine.hpp"
#include "resample/cubic.hpp"
#include "resample/hermite.hpp"
#include "resample/average.hpp"

void DSP::adjustVolume() {
  output.read(0) *= settings.volume;
  output.read(1) *= settings.volume;
}

void DSP::adjustBalance() {
  if(settings.balance < 0.0) output.read(1) *= 1.0 + settings.balance;
  if(settings.balance > 0.0) output.read(0) *= 1.0 - settings.balance;
}

signed DSP::clamp(const unsigned bits, const signed x) {
  const signed b = 1U << (bits - 1);
  const signed m = (1U << (bits - 1)) - 1;
  return (x > m) ? m : (x < -b) ? -b : x;
}

void DSP::clear() {
  resampler.fraction = 0.0;
  buffer.clear();
  output.clear();
}

DSP::DSP() {
  setPrecision(16);
  setFrequency(44100.0);
  setVolume(1.0);
  setBalance(0.0);
  setResampler(Resampler::Hermite);
  setResamplerFrequency(44100.0);
  clear();
}

DSP::~DSP() {
}

}

#endif
