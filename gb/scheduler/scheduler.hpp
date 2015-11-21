struct Scheduler {
  enum class SynchronizeMode : uint { None, CPU, All } sync;
  enum class ExitReason : uint { UnknownEvent, StepEvent, FrameEvent, SynchronizeEvent };

  auto init() -> void;
  auto enter() -> void;
  auto exit(ExitReason) -> void;

  cothread_t host_thread = nullptr;
  cothread_t active_thread = nullptr;
  ExitReason exit_reason = ExitReason::UnknownEvent;
};

extern Scheduler scheduler;
