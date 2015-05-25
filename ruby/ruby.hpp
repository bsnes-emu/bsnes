/* ruby
 * author: byuu
 * license: ISC
 * version: 0.12 (2015-05-24)
 *
 * ruby is a cross-platform hardware abstraction layer
 * it provides a common interface to video, audio and input devices
 */

#ifndef RUBY_HPP
#define RUBY_HPP

#include <nall/nall.hpp>

namespace ruby {

#include <ruby/video.hpp>
#include <ruby/audio.hpp>
#include <ruby/input.hpp>

struct VideoInterface {
  ~VideoInterface();

  auto driver(nall::string driver = "") -> void;
  auto optimalDriver() -> nall::string;
  auto safestDriver() -> nall::string;
  auto availableDrivers() -> nall::string;
  auto init() -> bool;
  auto term() -> void;

  auto cap(const nall::string& name) -> bool;
  auto get(const nall::string& name) -> nall::any;
  auto set(const nall::string& name, const nall::any& value) -> bool;

  auto lock(uint32_t*& data, unsigned& pitch, unsigned width, unsigned height) -> bool;
  auto unlock() -> void;
  auto clear() -> void;
  auto refresh() -> void;

private:
  Video* p = nullptr;
};

struct AudioInterface {
  ~AudioInterface();

  auto driver(nall::string driver = "") -> void;
  auto optimalDriver() -> nall::string;
  auto safestDriver() -> nall::string;
  auto availableDrivers() -> nall::string;
  auto init() -> bool;
  auto term() -> void;

  auto cap(const nall::string& name) -> bool;
  auto get(const nall::string& name) -> nall::any;
  auto set(const nall::string& name, const nall::any& value) -> bool;

  auto sample(uint16_t left, uint16_t right) -> void;
  auto clear() -> void;

private:
  Audio* p = nullptr;
};

struct InputInterface {
  nall::function<void (nall::shared_pointer<nall::HID::Device> device, unsigned group, unsigned input, int16_t oldValue, int16_t newValue)> onChange;

  ~InputInterface();

  auto driver(nall::string driver = "") -> void;
  auto optimalDriver() -> nall::string;
  auto safestDriver() -> nall::string;
  auto availableDrivers() -> nall::string;
  auto init() -> bool;
  auto term() -> void;

  auto cap(const nall::string& name) -> bool;
  auto get(const nall::string& name) -> nall::any;
  auto set(const nall::string& name, const nall::any& value) -> bool;

  auto acquire() -> bool;
  auto unacquire() -> bool;
  auto acquired() -> bool;
  auto poll() -> nall::vector<nall::shared_pointer<nall::HID::Device>>;
  auto rumble(uint64_t id, bool enable) -> bool;

private:
  Input* p = nullptr;
};

extern VideoInterface video;
extern AudioInterface audio;
extern InputInterface input;

};

#endif
