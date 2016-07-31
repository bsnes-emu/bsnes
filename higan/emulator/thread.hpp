#pragma once

namespace Emulator {

struct Thread {
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
    _scalar = ((uint128_t)1 << 96) / _frequency;
  }

  auto setScalar(uint128_t scalar) -> void {
    _scalar = scalar;
  }

  auto setClock(uint128_t clock) -> void {
    _clock = clock;
  }

  auto create(auto (*entrypoint)() -> void, double frequency) -> void {
    if(_handle) co_delete(_handle);
    _handle = co_create(64 * 1024 * sizeof(void*), entrypoint);
    setFrequency(frequency);
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
  uint32_t _frequency = 0;
  uint128_t _scalar = 0;
  uint128_t _clock = 0;

  friend class Scheduler;
};

}
