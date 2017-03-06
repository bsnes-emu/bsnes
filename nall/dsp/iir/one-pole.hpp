#pragma once

//one-pole first-order IIR filter

namespace nall { namespace DSP { namespace IIR {

struct OnePole {
  enum class Type : uint {
    LowPass,
    HighPass,
  };

  inline auto reset(Type type, double cutoffFrequency, double samplingFrequency) -> void;
  inline auto process(double in) -> double;  //normalized sample (-1.0 to +1.0)

private:
  Type type;
  double cutoffFrequency;
  double samplingFrequency;
  double a0, b1;  //coefficients
  double z1;      //first-order IIR
};

auto OnePole::reset(Type type, double cutoffFrequency, double samplingFrequency) -> void {
  this->type = type;
  this->cutoffFrequency = cutoffFrequency;
  this->samplingFrequency = samplingFrequency;

  b1 = exp(-2.0 * Math::Pi * cutoffFrequency / samplingFrequency);
  a0 = 1.0 - b1;

  z1 = 0.0;
}

auto OnePole::process(double in) -> double {
  z1 = in * a0 + z1 * b1;

  switch(type) {
  case Type::LowPass: return z1;
  case Type::HighPass: return in - z1;
  default: return 0.0;
  }
}

}}}
