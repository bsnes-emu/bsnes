#pragma once

namespace Emulator {

struct Thread {
  enum : uintmax { Second = (uintmax)1 << (8 * sizeof(uintmax) - 1) };

  virtual ~Thread() {
    if(_handle) co_delete(_handle);
  }

  inline auto active() const { return co_active() == _handle; }
  inline auto handle() const { return _handle; }
  inline auto frequency() const { return _frequency; }
  inline auto scalar() const { return _scalar; }
  inline auto clock() const { return _clock; }

  auto setFrequency(double frequency) -> void {
    _frequency = frequency + 0.5;
    _scalar = Second / _frequency;
  }

  auto setScalar(uintmax scalar) -> void {
    _scalar = scalar;
  }

  auto setClock(uintmax clock) -> void {
    _clock = clock;
  }

  auto create(auto (*entrypoint)() -> void, double frequency) -> void {
    if(_handle) co_delete(_handle);
    _handle = co_create(64 * 1024 * sizeof(void*), entrypoint);
    setFrequency(frequency);
    setClock(0);
  }

  inline auto step(uint clocks) -> void {
    _clock += _scalar * clocks;
  }

  auto serialize(serializer& s) -> void {
    s.integer(_frequency);
    s.integer(_scalar);
    s.integer(_clock);
  }

protected:
  cothread_t _handle = nullptr;
  uintmax _frequency = 0;
  uintmax _scalar = 0;
  uintmax _clock = 0;

  friend class Scheduler;
};

}
