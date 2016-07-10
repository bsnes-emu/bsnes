#pragma once

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

  auto reset(cothread_t master_) -> void {
    master = resume = master_;
    host = co_active();
  }

  auto enter(Mode mode_ = Mode::Run) -> Event {
    mode = mode_;
    host = co_active();
    co_switch(resume);
    return event;
  }

  auto exit(Event event_) -> void {
    event = event_;
    resume = co_active();
    co_switch(host);
  }

  auto synchronize(cothread_t thread) -> void {
    if(thread == master) {
      while(enter(Mode::SynchronizeMaster) != Event::Synchronize);
    } else {
      resume = thread;
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

  auto synchronizing() const -> bool {
    return mode == Mode::SynchronizeSlave;
  }

private:
  cothread_t host = nullptr;    //program thread (used to exit scheduler)
  cothread_t resume = nullptr;  //resume thread (used to enter scheduler)
  cothread_t master = nullptr;  //primary thread (used to synchronize components)
  Mode mode = Mode::Run;
  Event event = Event::Step;
};

}
