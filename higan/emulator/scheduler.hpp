#pragma once

namespace Emulator {

struct Thread;

struct Scheduler {
  enum class Mode : uint {
    Run,
    SynchronizeMaster,
    SynchronizeSlave,
  };

  enum class Event : uint {
    Step,
    Frame,
    Synchronize,
  };

  auto active(Thread& thread) const -> bool {
    return co_active() == thread.handle();
  }

  auto reset() -> void {
    threads.reset();
  }

  auto primary(Thread& thread) -> void {
    master = _resume = thread.handle();
    host = co_active();
  }

  auto append(Thread& thread) -> bool {
    if(threads.find(&thread)) return false;
    return threads.append(&thread), true;
  }

  auto remove(Thread& thread) -> bool {
    if(auto offset = threads.find(&thread)) return threads.remove(*offset), true;
    return false;
  }

  auto enter(Mode mode_ = Mode::Run) -> Event {
    mode = mode_;
    host = co_active();
    co_switch(_resume);
    return event;
  }

  inline auto resume(Thread& thread) -> void {
    if(mode != Mode::SynchronizeSlave) co_switch(thread.handle());
  }

  auto exit(Event event_) -> void {
    uint64 minimum = ~0ull >> 1;
    for(auto thread : threads) {
      if(thread->_clock < minimum) minimum = thread->_clock;
    }
    for(auto thread : threads) {
      thread->_clock -= minimum;
    }

    event = event_;
    _resume = co_active();
    co_switch(host);
  }

  auto synchronize(Thread& thread) -> void {
    if(thread.handle() == master) {
      while(enter(Mode::SynchronizeMaster) != Event::Synchronize);
    } else {
      _resume = thread.handle();
      while(enter(Mode::SynchronizeSlave) != Event::Synchronize);
    }
  }

  auto synchronize() -> void {
    if(co_active() == master) {
      if(mode == Mode::SynchronizeMaster) return exit(Event::Synchronize);
    } else {
      if(mode == Mode::SynchronizeSlave) return exit(Event::Synchronize);
    }
  }

private:
  cothread_t host = nullptr;    //program thread (used to exit scheduler)
  cothread_t _resume = nullptr;  //resume thread (used to enter scheduler)
  cothread_t master = nullptr;  //primary thread (used to synchronize components)
  Mode mode = Mode::Run;
  Event event = Event::Step;
  vector<Thread*> threads;
};

}
