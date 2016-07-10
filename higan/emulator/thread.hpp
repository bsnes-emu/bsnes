#pragma once

namespace Emulator {

struct Thread {
  virtual ~Thread() {
    if(thread) co_delete(thread);
  }

  auto create(auto (*entrypoint)() -> void, double frequency_) -> void {
    if(thread) co_delete(thread);
    thread = co_create(64 * 1024 * sizeof(void*), entrypoint);
    frequency = frequency_ + 0.5;  //round to nearest whole number
    clock = 0;
  }

  auto serialize(serializer& s) -> void {
    s.integer(frequency);
    s.integer(clock);
  }

  cothread_t thread = nullptr;
  uint frequency = 0;
  int64 clock = 0;
};

}
