struct Scheduler : property<Scheduler> {
  enum class SynchronizeMode : uint { None, CPU, All } sync;
  enum class ExitReason : uint { UnknownEvent, FrameEvent, SynchronizeEvent };
  readonly<ExitReason> exit_reason;

  cothread_t host;
  cothread_t active;

  Scheduler();
  auto enter() -> void;
  auto exit(ExitReason) -> void;
  auto power() -> void;
};

extern Scheduler scheduler;
