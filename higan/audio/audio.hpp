#pragma once

#include <nall/dsp/iir/biquad.hpp>
#include <nall/dsp/resampler/cubic.hpp>

namespace Emulator {

struct Interface;
struct Audio;
struct Stream;

struct Audio {
  auto reset() -> void;
  auto setInterface(Interface*) -> void;

  auto setFrequency(double frequency) -> void;
  auto setVolume(double volume) -> void;
  auto setBalance(double balance) -> void;
  auto setReverb(bool enabled) -> void;

  auto createStream(uint channels, double frequency) -> shared_pointer<Stream>;

  auto poll() -> void;

private:
  Interface* interface = nullptr;
  vector<shared_pointer<Stream>> streams;
  double frequency = 0.0;
  double volume = 1.0;
  double balance = 0.0;

  bool reverbEnable = false;
  vector<vector<queue<int16>>> reverb;

  friend class Stream;
};

struct Stream {
  Stream(uint channels, double inputFrequency);

  auto reset() -> void;
  auto setFrequency(double outputFrequency) -> void;

  auto pending() const -> bool;
  auto read(double* samples) -> void;
  auto write(int16* samples) -> void;

  template<typename... P> auto sample(P&&... p) -> void {
    int16 samples[sizeof...(P)] = {forward<P>(p)...};
    write(samples);
  }

private:
  const uint channels;
  const double inputFrequency;
  double outputFrequency = 0.0;
  double cutoffFrequency = 0.0;

  const uint iirPasses = 3;  //6th-order filter
  vector<vector<DSP::IIR::Biquad>> iir;
  vector<DSP::Resampler::Cubic> resampler;

  friend class Audio;
};

extern Audio audio;

}
