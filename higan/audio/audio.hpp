#pragma once

#include <nall/dsp/iir/biquad.hpp>
#include <nall/dsp/resampler/cubic.hpp>

namespace Emulator {

struct Interface;
struct Audio;
struct Stream;

struct Audio {
  auto reset(maybe<uint> channels = nothing, maybe<double> frequency = nothing) -> void;
  auto setInterface(Interface* interface) -> void;

  auto setVolume(double volume) -> void;
  auto setBalance(double balance) -> void;
  auto setReverb(bool enabled) -> void;

  auto createStream(uint channels, double frequency) -> shared_pointer<Stream>;

private:
  auto process() -> void;

  Interface* interface = nullptr;
  vector<shared_pointer<Stream>> streams;

  uint channels = 0;
  double frequency = 0.0;

  double volume = 1.0;
  double balance = 0.0;

  bool reverbEnable = false;
  vector<vector<queue<double>>> reverb;

  friend class Stream;
};

struct Stream {
  auto reset(uint channels, double inputFrequency, double outputFrequency) -> void;

  auto pending() const -> bool;
  auto read(double* samples) -> uint;
  auto write(const double* samples) -> void;

  template<typename... P> auto sample(P&&... p) -> void {
    double samples[sizeof...(P)] = {forward<P>(p)...};
    write(samples);
  }

private:
  const uint order = 6;  //Nth-order filter (must be an even number)
  struct Channel {
    vector<DSP::IIR::Biquad> iir;
    DSP::Resampler::Cubic resampler;
  };
  vector<Channel> channels;

  friend class Audio;
};

extern Audio audio;

}
