struct Scheduler {
  enum class SynchronizeMode : uint { None, CPU, All };
  enum class ExitReason : uint { UnknownEvent, FrameEvent, SynchronizeEvent };

  auto enter() -> void;
  auto exit(ExitReason reason) -> void;
  auto power() -> void;

  cothread_t host = nullptr;
  cothread_t active = nullptr;
  SynchronizeMode sync = SynchronizeMode::None;
  ExitReason exitReason = ExitReason::UnknownEvent;
};

extern Scheduler scheduler;
