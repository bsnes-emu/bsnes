#pragma once

#include "core.hpp"

namespace Emulator {

struct Interface;

struct Stream {
  Stream(double inputFrequency, double outputFrequency, double volume, double balance);
  auto sample(int16 left, int16 right) -> void;

private:
  nall::DSP dsp;

  friend class Audio;
};

struct Audio {
  auto reset() -> void;
  auto setInterface(Interface*) -> void;

  auto setFrequency(double frequency) -> void;
  auto setVolume(double volume) -> void;
  auto setBalance(double balance) -> void;
  auto setReverbDelay(uint milliseconds) -> void;
  auto setReverbLevel(double level) -> void;

  auto createStream(double frequency) -> shared_pointer<Stream>;

  auto poll() -> void;

private:
  Interface* interface = nullptr;
  vector<shared_pointer<Stream>> streams;
  double frequency = 0.0;
  double volume = 1.0;
  double balance = 0.0;
  uint reverbDelay = 0;  //0 = disabled
  double reverbLevel = 0.0;
  vector<int16> reverbLeft;
  vector<int16> reverbRight;

  friend class Stream;
};

extern Audio audio;

}
