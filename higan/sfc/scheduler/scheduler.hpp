struct Scheduler {
  enum class SynchronizeMode : uint { None, CPU, All } sync;
  enum class ExitReason : uint { UnknownEvent, FrameEvent, SynchronizeEvent, DebuggerEvent };

  auto init() -> void;
  auto enter() -> void;
  auto exit(ExitReason) -> void;
  auto debug() -> void;

  cothread_t host_thread = nullptr;  //program thread (used to exit emulation)
  cothread_t thread = nullptr;       //active emulation thread (used to enter emulation)
  ExitReason exit_reason = ExitReason::UnknownEvent;
};

extern Scheduler scheduler;
