#ifndef NALL_DSP_EFFECT_HPP
#define NALL_DSP_EFFECT_HPP

#include <nall/stdint.hpp>

namespace nall {

struct dsp {
  inline void set_precision(unsigned precision);
  inline void set_frequency(double frequency);
  inline void set_volume(double volume);
  inline void set_balance(double balance);
  inline void set_echo(double echo);

  inline void set_resampler_frequency(double frequency);

  inline void sample(signed lchannel, signed rchannel);
  inline bool pending();
  inline void read(signed &lchannel, signed &rchannel);

  inline signed clamp(const unsigned bits, const signed x);
  inline void clear();
  inline dsp();
  inline ~dsp();

protected:
  struct Settings {
    unsigned precision;
    double frequency;
    double volume;
    double balance;
    double echo;
    //internal
    double intensity;
  } settings;

  struct Resampler {
    double frequency;
    //internal
    double fraction;
    double step;
  } resampler;

  inline void resampler_run();

  struct Buffer {
    double *sample[2];
    uint16_t rdoffset;
    uint16_t wroffset;
    inline double& operator()(bool channel, signed offset) { return sample[channel][(uint16_t)(rdoffset + offset)]; }
    inline void write(bool channel, signed offset, double data) { sample[channel][(uint16_t)(wroffset + offset)] = data; }
  } buffer;

  struct Output {
    double *sample[2];
    uint16_t rdoffset;
    uint16_t wroffset;
    inline double& operator()(bool channel, signed offset) { return sample[channel][(uint16_t)(rdoffset + offset)]; }
    inline void write(bool channel, signed offset, double data) { sample[channel][(uint16_t)(wroffset + offset)] = data; }
  } output;

  inline void adjust_volume();
  inline void adjust_balance();
  inline void adjust_echo();
};

void dsp::set_precision(unsigned precision) {
  settings.precision = precision;
  settings.intensity = 1 << (settings.precision - 1);
}

void dsp::set_frequency(double frequency) {
  settings.frequency = frequency;
  resampler.fraction = 0;
  resampler.step = settings.frequency / resampler.frequency;
}

void dsp::set_volume(double volume) {
  settings.volume = volume;
}

void dsp::set_balance(double balance) {
  settings.balance = balance;
}

void dsp::set_echo(double echo) {
  settings.echo = echo;
}

void dsp::set_resampler_frequency(double frequency) {
  resampler.frequency = frequency;
  resampler.fraction = 0;
  resampler.step = settings.frequency / resampler.frequency;
}

void dsp::sample(signed lchannel, signed rchannel) {
  buffer.write(0, 0, (double)lchannel / settings.intensity);
  buffer.write(1, 0, (double)rchannel / settings.intensity);
  buffer.wroffset++;

#ifdef DSP_NO_RESAMPLER
  output.write(0, 0, buffer(0, 0));
  output.write(1, 0, buffer(1, 0));
  output.wroffset++;
  buffer.rdoffset++;
#else
  resampler_run();
#endif
}

bool dsp::pending() {
  return output.rdoffset != output.wroffset;
}

void dsp::read(signed &lchannel, signed &rchannel) {
  adjust_volume();
  adjust_balance();
  adjust_echo();

  lchannel = clamp(settings.precision, output(0, 0) * settings.intensity);
  rchannel = clamp(settings.precision, output(1, 0) * settings.intensity);
  output.rdoffset++;
}

void dsp::resampler_run() {
  //4-tap hermite
  while(resampler.fraction <= 1.0) {
    for(unsigned n = 0; n < 2; n++) {
      double a = buffer(n, -3);
      double b = buffer(n, -2);
      double c = buffer(n, -1);
      double d = buffer(n, -0);

      const double tension = 0.0;  //-1 = low, 0 = normal, +1 = high
      const double bias = 0.0;  //-1 = left, 0 = even, +1 = right

      double mu1, mu2, mu3, m0, m1, a0, a1, a2, a3;

      mu1 = resampler.fraction;
      mu2 = mu1 * mu1;
      mu3 = mu2 * mu1;

      m0  = (b - a) * (1.0 + bias) * (1.0 - tension) / 2.0;
      m0 += (c - b) * (1.0 - bias) * (1.0 - tension) / 2.0;
      m1  = (c - b) * (1.0 + bias) * (1.0 - tension) / 2.0;
      m1 += (d - c) * (1.0 - bias) * (1.0 - tension) / 2.0;

      a0 = +2 * mu3 - 3 * mu2 + 1;
      a1 =      mu3 - 2 * mu2 + mu1;
      a2 =      mu3 -     mu2;
      a3 = -2 * mu3 + 3 * mu2;

      double result = (a0 * b) + (a1 * m0) + (a2 * m1) + (a3 * c);
      output.write(n, 0, result);
    }

    output.wroffset++;
    resampler.fraction += resampler.step;
  }

  buffer.rdoffset++;
  resampler.fraction -= 1.0;
}

void dsp::adjust_volume() {
  output(0, 0) *= settings.volume;
  output(1, 0) *= settings.volume;
}

void dsp::adjust_balance() {
  if(settings.balance < 0.0) output(1, 0) *= 1.0 + settings.balance;
  if(settings.balance > 0.0) output(0, 0) *= 1.0 - settings.balance;
}

void dsp::adjust_echo() {
  //...
}

signed dsp::clamp(const unsigned bits, const signed x) {
  const signed b = 1U << (bits - 1);
  const signed m = (1U << (bits - 1)) - 1;
  return (x > m) ? m : (x < -b) ? -b : x;
}

void dsp::clear() {
  resampler.fraction = 0.0;
  resampler.step = 1.0;
  for(unsigned n = 0; n < 65536; n++) {
    buffer.sample[0][n] = 0.0;
    buffer.sample[1][n] = 0.0;
    output.sample[0][n] = 0.0;
    output.sample[1][n] = 0.0;
  }
  buffer.rdoffset = 0;
  buffer.wroffset = 0;
  output.rdoffset = 0;
  output.wroffset = 0;
}

dsp::dsp() {
  settings.precision  = 16;
  settings.frequency  = 44100.0;
  settings.volume     = 1.0;
  settings.balance    = 0.0;
  settings.echo       = 0.0;
  settings.intensity  = 1 << (settings.precision - 1);
  resampler.frequency = 44100.0;

  buffer.sample[0] = new double[65536];
  buffer.sample[1] = new double[65536];
  output.sample[0] = new double[65536];
  output.sample[1] = new double[65536];

  clear();
}

dsp::~dsp() {
  delete[] buffer.sample[0];
  delete[] buffer.sample[1];
  delete[] output.sample[0];
  delete[] output.sample[1];
}

}

#endif
