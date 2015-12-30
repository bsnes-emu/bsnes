struct Scheduler : property<Scheduler> {
  enum class SynchronizeMode : uint { None, PPU, All } sync;
  enum class ExitReason : uint { UnknownEvent, FrameEvent, SynchronizeEvent };

  auto enter() -> void;
  auto exit(ExitReason) -> void;

  auto power() -> void;
  auto reset() -> void;

  cothread_t host_thread;  //program thread (used to exit emulation)
  cothread_t thread;       //active emulation thread (used to enter emulation)
  readonly<ExitReason> exit_reason;
};

extern Scheduler scheduler;
