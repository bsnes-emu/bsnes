struct Scheduler {
  enum class Mode : uint {
    Run,
    SynchronizeCPU,
    SynchronizeAll,
  };

  enum class Event : uint {
    Unknown,
    Frame,
    Synchronize,
    Debugger,
  };

  auto reset() -> void;
  auto enter(Mode = Mode::Run) -> Event;
  auto exit(Event) -> void;
  auto synchronize(cothread_t) -> void;
  auto synchronize() -> void;
  auto synchronizing() const -> bool;

private:
  cothread_t host = nullptr;     //program thread (used to exit emulation)
  cothread_t resume = nullptr;   //resume thread (used to re-enter emulation)
  Mode mode = Mode::Run;         //determines when to exit emulation thread
  Event event = Event::Unknown;  //set by exit(), returned by enter()
};

extern Scheduler scheduler;
