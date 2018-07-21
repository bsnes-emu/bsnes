#pragma once

#include <nall/dsp/dsp.hpp>

//transposed direct form II biquadratic second-order IIR filter

namespace nall { namespace DSP { namespace IIR {

struct Biquad {
  enum class Type : uint {
    LowPass,
    HighPass,
    BandPass,
    Notch,
    Peak,
    LowShelf,
    HighShelf,
  };

  inline auto reset(Type type, real cutoffFrequency, real samplingFrequency, real quality, real gain = 0.0) -> void;
  inline auto process(real in) -> real;  //normalized sample (-1.0 to +1.0)

  inline static auto butterworth(uint order, uint phase) -> real;

private:
  Type type;
  real cutoffFrequency;
  real samplingFrequency;
  real quality;             //frequency response quality
  real gain;                //peak gain
  real a0, a1, a2, b1, b2;  //coefficients
  real z1, z2;              //second-order IIR
};

auto Biquad::reset(Type type, real cutoffFrequency, real samplingFrequency, real quality, real gain) -> void {
  this->type = type;
  this->cutoffFrequency = cutoffFrequency;
  this->samplingFrequency = samplingFrequency;
  this->quality = quality;
  this->gain = gain;

  z1 = 0.0;
  z2 = 0.0;

  real v = pow(10, fabs(gain) / 20.0);
  real k = tan(Math::Pi * cutoffFrequency / samplingFrequency);
  real q = quality;
  real n = 0.0;

  switch(type) {

  case Type::LowPass:
    n = 1 / (1 + k / q + k * k);
    a0 = k * k * n;
    a1 = 2 * a0;
    a2 = a0;
    b1 = 2 * (k * k - 1) * n;
    b2 = (1 - k / q + k * k) * n;
    break;

  case Type::HighPass:
    n = 1 / (1 + k / q + k * k);
    a0 = 1 * n;
    a1 = -2 * a0;
    a2 = a0;
    b1 = 2 * (k * k - 1) * n;
    b2 = (1 - k / q + k * k) * n;
    break;

  case Type::BandPass:
    n = 1 / (1 + k / q + k * k);
    a0 = k / q * n;
    a1 = 0;
    a2 = -a0;
    b1 = 2 * (k * k - 1) * n;
    b2 = (1 - k / q + k * k) * n;
    break;

  case Type::Notch:
    n = 1 / (1 + k / q + k * k);
    a0 = (1 + k * k) * n;
    a1 = 2 * (k * k - 1) * n;
    a2 = a0;
    b1 = a1;
    b2 = (1 - k / q + k * k) * n;
    break;

  case Type::Peak:
    if(gain >= 0) {
      n = 1 / (1 + 1 / q * k + k * k);
      a0 = (1 + v / q * k + k * k) * n;
      a1 = 2 * (k * k - 1) * n;
      a2 = (1 - v / q * k + k * k) * n;
      b1 = a1;
      b2 = (1 - 1 / q * k + k * k) * n;
    } else {
      n = 1 / (1 + v / q * k + k * k);
      a0 = (1 + 1 / q * k + k * k) * n;
      a1 = 2 * (k * k - 1) * n;
      a2 = (1 - 1 / q * k + k * k) * n;
      b1 = a1;
      b2 = (1 - v / q * k + k * k) * n;
    }
    break;

  case Type::LowShelf:
    if(gain >= 0) {
      n = 1 / (1 + sqrt(2) * k + k * k);
      a0 = (1 + sqrt(2 * v) * k + v * k * k) * n;
      a1 = 2 * (v * k * k - 1) * n;
      a2 = (1 - sqrt(2 * v) * k + v * k * k) * n;
      b1 = 2 * (k * k - 1) * n;
      b2 = (1 - sqrt(2) * k + k * k) * n;
    } else {
      n = 1 / (1 + sqrt(2 * v) * k + v * k * k);
      a0 = (1 + sqrt(2) * k + k * k) * n;
      a1 = 2 * (k * k - 1) * n;
      a2 = (1 - sqrt(2) * k + k * k) * n;
      b1 = 2 * (v * k * k - 1) * n;
      b2 = (1 - sqrt(2 * v) * k + v * k * k) * n;
    }
    break;

  case Type::HighShelf:
    if(gain >= 0) {
      n = 1 / (1 + sqrt(2) * k + k * k);
      a0 = (v + sqrt(2 * v) * k + k * k) * n;
      a1 = 2 * (k * k - v) * n;
      a2 = (v - sqrt(2 * v) * k + k * k) * n;
      b1 = 2 * (k * k - 1) * n;
      b2 = (1 - sqrt(2) * k + k * k) * n;
    } else {
      n = 1 / (v + sqrt(2 * v) * k + k * k);
      a0 = (1 + sqrt(2) * k + k * k) * n;
      a1 = 2 * (k * k - 1) * n;
      a2 = (1 - sqrt(2) * k + k * k) * n;
      b1 = 2 * (k * k - v) * n;
      b2 = (v - sqrt(2 * v) * k + k * k) * n;
    }
    break;

  }
}

auto Biquad::process(real in) -> real {
  real out = in * a0 + z1;
  z1 = in * a1 + z2 - b1 * out;
  z2 = in * a2 - b2 * out;
  return out;
}

//compute Q values for N-order butterworth filtering
auto Biquad::butterworth(uint order, uint phase) -> real {
  return -0.5 / cos(Math::Pi * (phase + order + 0.5) / order);
}

}}}
