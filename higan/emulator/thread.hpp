#pragma once

namespace Emulator {

struct Thread {
  virtual ~Thread() {
    if(_handle) co_delete(_handle);
  }

  auto handle() const { return _handle; }
  auto frequency() const { return _frequency; }
  auto scalar() const { return _scalar; }
  auto clock() const { return _clock; }

  auto create(auto (*entrypoint)() -> void, double frequency, bool resetClock = true) -> void {
    if(_handle) co_delete(_handle);
    _handle = co_create(64 * 1024 * sizeof(void*), entrypoint);
    if(resetClock) _clock = 0;
    setFrequency(frequency);
  }

  auto setFrequency(double frequency) -> void {
    _frequency = frequency;
    _scalar = 1.0L / frequency * Constants::Time::Attosecond + 0.5L;
  }

  inline auto step(uint clocks) -> void {
    _clock += _scalar * clocks;
  }

  inline auto synchronize(Thread& thread) -> void {
    if(_clock > thread._clock) co_switch(thread._handle);
  }

  auto serialize(serializer& s) -> void {
    s.integer(_frequency);
    s.integer(_scalar);
    s.integer(_clock);
  }

protected:
  cothread_t _handle = nullptr;
  uint64 _frequency = 0;
  uint64 _scalar = 0;
  uint64 _clock = 0;

  friend class Scheduler;
};

}
