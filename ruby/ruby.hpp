/* ruby
 * author: byuu
 * license: ISC
 * version: 0.13 (2015-06-18)
 *
 * ruby is a cross-platform hardware abstraction layer
 * it provides a common interface to video, audio and input devices
 */

#ifndef RUBY_HPP
#define RUBY_HPP

#include <nall/nall.hpp>

namespace ruby {

struct Video {
  static const nall::string Handle;
  static const nall::string Synchronize;
  static const nall::string Depth;
  static const nall::string Filter;
  static const nall::string Shader;

  static const unsigned FilterNearest;
  static const unsigned FilterLinear;

  static auto create(const nall::string& driver = "") -> Video*;
  static auto optimalDriver() -> nall::string;
  static auto safestDriver() -> nall::string;
  static auto availableDrivers() -> nall::lstring;

  virtual ~Video() = default;

  virtual auto cap(const nall::string& name) -> bool { return false; }
  virtual auto get(const nall::string& name) -> nall::any { return false; }
  virtual auto set(const nall::string& name, const nall::any& value) -> bool { return false; }

  virtual auto lock(uint32_t*& data, unsigned& pitch, unsigned width, unsigned height) -> bool { return false; }
  virtual auto unlock() -> void {}
  virtual auto clear() -> void {}
  virtual auto refresh() -> void {}

  virtual auto init() -> bool { return true; }
  virtual auto term() -> void {}
};

struct Audio {
  static const nall::string Device;
  static const nall::string Handle;
  static const nall::string Synchronize;
  static const nall::string Frequency;
  static const nall::string Latency;

  static auto create(const nall::string& driver = "") -> Audio*;
  static auto optimalDriver() -> nall::string;
  static auto safestDriver() -> nall::string;
  static auto availableDrivers() -> nall::lstring;

  virtual ~Audio() = default;

  virtual auto cap(const nall::string& name) -> bool { return false; }
  virtual auto get(const nall::string& name) -> nall::any { return false; }
  virtual auto set(const nall::string& name, const nall::any& value) -> bool { return false; }

  virtual auto sample(uint16_t left, uint16_t right) -> void {}
  virtual auto clear() -> void {}

  virtual auto init() -> bool { return true; }
  virtual auto term() -> void {}
};

struct Input {
  static const nall::string Handle;
  static const nall::string KeyboardSupport;
  static const nall::string MouseSupport;
  static const nall::string JoypadSupport;
  static const nall::string JoypadRumbleSupport;

  static auto create(const nall::string& driver = "") -> Input*;
  static auto optimalDriver() -> nall::string;
  static auto safestDriver() -> nall::string;
  static auto availableDrivers() -> nall::lstring;

  virtual ~Input() = default;

  virtual auto cap(const nall::string& name) -> bool { return false; }
  virtual auto get(const nall::string& name) -> nall::any { return false; }
  virtual auto set(const nall::string& name, const nall::any& value) -> bool { return false; }

  virtual auto acquire() -> bool { return false; }
  virtual auto release() -> bool { return false; }
  virtual auto acquired() -> bool { return false; }
  virtual auto poll() -> nall::vector<nall::shared_pointer<nall::HID::Device>> { return {}; }
  virtual auto rumble(uint64_t id, bool enable) -> bool { return false; }

  virtual auto init() -> bool { return true; }
  virtual auto term() -> void {}

  auto onChange(const nall::function<void (nall::shared_pointer<nall::HID::Device>, unsigned, unsigned, int16_t, int16_t)>& callback) { _onChange = callback; }
  auto doChange(nall::shared_pointer<nall::HID::Device> device, unsigned group, unsigned input, int16_t oldValue, int16_t newValue) -> void {
    if(_onChange) _onChange(device, group, input, oldValue, newValue);
  }

private:
  nall::function<void (nall::shared_pointer<nall::HID::Device> device, unsigned group, unsigned input, int16_t oldValue, int16_t newValue)> _onChange;
};

}

#endif
