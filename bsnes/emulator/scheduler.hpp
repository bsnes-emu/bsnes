#pragma once
#define uintmax uint64

namespace Emulator {

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

  inline auto synchronizing() const -> bool { return _mode == Mode::SynchronizeSlave; }

  auto reset() -> void {
    _host = co_active();
    _threads.reset();
  }

  auto primary(Thread& thread) -> void {
    _master = _resume = thread.handle();
    uintmax clock = 0;
    for(auto& thread : _threads) {
      thread->_clock = clock++;  //this bias prioritizes threads appended earlier first
    }
  }

  auto append(Thread& thread) -> bool {
    if(_threads.find(&thread)) return false;
    thread._clock = _threads.size();
    return _threads.append(&thread), true;
  }

  auto remove(Thread& thread) -> bool {
    if(auto offset = _threads.find(&thread)) return _threads.remove(*offset), true;
    return false;
  }

  auto enter(Mode mode = Mode::Run) -> Event {
    _mode = mode;
    _host = co_active();
    co_switch(_resume);
    return _event;
  }

  inline auto resume(Thread& thread) -> void {
    if(_mode != Mode::SynchronizeSlave) co_switch(thread.handle());
  }

  auto exit(Event event) -> void {
    uintmax minimum = -1;
    for(auto thread : _threads) {
      if(thread->_clock < minimum) minimum = thread->_clock;
    }
    for(auto thread : _threads) {
      thread->_clock -= minimum;
    }

    _event = event;
    _resume = co_active();
    co_switch(_host);
  }

  inline auto synchronize(Thread& thread) -> void {
    if(thread.handle() == _master) {
      while(enter(Mode::SynchronizeMaster) != Event::Synchronize);
    } else {
      _resume = thread.handle();
      while(enter(Mode::SynchronizeSlave) != Event::Synchronize);
    }
  }

  inline auto synchronize() -> void {
    if(co_active() == _master) {
      if(_mode == Mode::SynchronizeMaster) return exit(Event::Synchronize);
    } else {
      if(_mode == Mode::SynchronizeSlave) return exit(Event::Synchronize);
    }
  }

private:
  cothread_t _host = nullptr;    //program thread (used to exit scheduler)
  cothread_t _resume = nullptr;  //resume thread (used to enter scheduler)
  cothread_t _master = nullptr;  //primary thread (used to synchronize components)
  Mode _mode = Mode::Run;
  Event _event = Event::Step;
  vector<Thread*> _threads;
};

}

#undef uintmax
