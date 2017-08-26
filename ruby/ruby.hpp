#pragma once

/* ruby
 * author: byuu
 * license: ISC
 * version: 0.16 (2017-07-08)
 *
 * ruby is a cross-platform hardware abstraction layer.
 * it provides a common interface to video, audio and input devices.
 */

#include <nall/nall.hpp>

namespace ruby {

struct Video {
  static auto create(const nall::string& driver = "") -> Video*;
  static auto optimalDriver() -> nall::string;
  static auto safestDriver() -> nall::string;
  static auto availableDrivers() -> nall::string_vector;

  struct Information {
  };

  virtual ~Video() = default;

  virtual auto ready() -> bool { return true; }
  virtual auto information() -> Information { return {}; }

  virtual auto exclusive() -> bool { return false; }
  virtual auto context() -> uintptr { return 0; }
  virtual auto blocking() -> bool { return false; }
  virtual auto depth() -> uint { return 24; }
  virtual auto smooth() -> bool { return false; }
  virtual auto shader() -> nall::string { return ""; }

  virtual auto setExclusive(bool exclusive) -> bool { return false; }
  virtual auto setContext(uintptr context) -> bool { return false; }
  virtual auto setBlocking(bool blocking) -> bool { return false; }
  virtual auto setDepth(uint depth) -> bool { return false; }
  virtual auto setSmooth(bool smooth) -> bool { return false; }
  virtual auto setShader(nall::string shader) -> bool { return false; }

  virtual auto clear() -> void {}
  virtual auto lock(uint32_t*& data, uint& pitch, uint width, uint height) -> bool { return false; }
  virtual auto unlock() -> void {}
  virtual auto output() -> void {}
};

struct Audio {
  static auto create(const nall::string& driver = "") -> Audio*;
  static auto optimalDriver() -> nall::string;
  static auto safestDriver() -> nall::string;
  static auto availableDrivers() -> nall::string_vector;

  virtual ~Audio() = default;

  virtual auto availableDevices() -> nall::string_vector { return {"Default"}; }
  virtual auto availableFrequencies() -> nall::vector<double> { return {44100.0}; }
  virtual auto availableLatencies() -> nall::vector<uint> { return {0}; }
  virtual auto availableChannels() -> nall::vector<uint> { return {2}; }

  virtual auto ready() -> bool { return true; }
  virtual auto exclusive() -> bool { return false; }
  virtual auto context() -> uintptr { return 0; }
  virtual auto device() -> nall::string { return "None"; }
  virtual auto blocking() -> bool { return false; }
  virtual auto channels() -> uint { return 2; }
  virtual auto frequency() -> double { return 48000.0; }
  virtual auto latency() -> uint { return 0; }

  virtual auto setExclusive(bool exclusive) -> bool { return false; }
  virtual auto setContext(uintptr context) -> bool { return false; }
  virtual auto setDevice(nall::string device) -> bool { return false; }
  virtual auto setBlocking(bool blocking) -> bool { return false; }
  virtual auto setChannels(uint channels) -> bool { return false; }
  virtual auto setFrequency(double frequency) -> bool { return false; }
  virtual auto setLatency(uint latency) -> bool { return false; }

  virtual auto clear() -> void {}
  virtual auto output(const double samples[]) -> void {}
};

struct Input {
  static auto create(const nall::string& driver = "") -> Input*;
  static auto optimalDriver() -> nall::string;
  static auto safestDriver() -> nall::string;
  static auto availableDrivers() -> nall::string_vector;

  struct Information {
  };

  virtual ~Input() = default;

  virtual auto ready() -> bool { return true; }
  virtual auto information() -> Information { return {}; }

  virtual auto context() -> uintptr { return 0; }

  virtual auto setContext(uintptr context) -> bool { return false; }

  virtual auto acquired() -> bool { return false; }
  virtual auto acquire() -> bool { return false; }
  virtual auto release() -> bool { return false; }
  virtual auto poll() -> nall::vector<nall::shared_pointer<nall::HID::Device>> { return {}; }
  virtual auto rumble(uint64_t id, bool enable) -> bool { return false; }

  auto onChange(const nall::function<void (nall::shared_pointer<nall::HID::Device>, uint, uint, int16_t, int16_t)>& callback) { _onChange = callback; }
  auto doChange(nall::shared_pointer<nall::HID::Device> device, uint group, uint input, int16_t oldValue, int16_t newValue) -> void {
    if(_onChange) _onChange(device, group, input, oldValue, newValue);
  }

private:
  nall::function<void (nall::shared_pointer<nall::HID::Device> device, uint group, uint input, int16_t oldValue, int16_t newValue)> _onChange;
};

}
