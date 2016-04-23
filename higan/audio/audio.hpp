#pragma once

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
  auto setReverbDelay(uint milliseconds) -> void;
  auto setReverbLevel(double level) -> void;

  auto createStream(uint channels, double frequency) -> shared_pointer<Stream>;

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

struct Stream {
  Stream(uint channels, double inputFrequency);
  ~Stream();

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

  double* tap = nullptr;
  uint taps = 0;

  uint decimationRate = 0;
  uint decimationOffset = 0;

  double** input = nullptr;
  uint inputOffset = 0;

  double resamplerFrequency = 0.0;
  double resamplerFraction = 0.0;
  double resamplerStep = 0.0;
  double** queue = nullptr;

  double** output = nullptr;
  uint outputs = 0;
  uint outputReadOffset = 0;
  uint outputWriteOffset = 0;

  friend class Audio;
};

extern Audio audio;

}
