#pragma once

#include <nall/dsp/dsp.hpp>

//one-pole first-order IIR filter

namespace nall { namespace DSP { namespace IIR {

struct OnePole {
  enum class Type : uint {
    LowPass,
    HighPass,
  };

  inline auto reset(Type type, real cutoffFrequency, real samplingFrequency) -> void;
  inline auto process(real in) -> real;  //normalized sample (-1.0 to +1.0)

private:
  Type type;
  real cutoffFrequency;
  real samplingFrequency;
  real a0, b1;  //coefficients
  real z1;      //first-order IIR
};

auto OnePole::reset(Type type, real cutoffFrequency, real samplingFrequency) -> void {
  this->type = type;
  this->cutoffFrequency = cutoffFrequency;
  this->samplingFrequency = samplingFrequency;

  z1 = 0.0;
  real x = cos(2.0 * Math::Pi * cutoffFrequency / samplingFrequency);
  if(type == Type::LowPass) {
    b1 = +2.0 - x - sqrt((+2.0 - x) * (+2.0 - x) - 1);
    a0 = 1.0 - b1;
  } else {
    b1 = -2.0 - x + sqrt((-2.0 - x) * (-2.0 - x) - 1);
    a0 = 1.0 + b1;
  }
}

auto OnePole::process(real in) -> real {
  return z1 = in * a0 + z1 * b1;
}

}}}
