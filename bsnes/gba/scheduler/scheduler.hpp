struct Scheduler : property<Scheduler> {
  enum class SynchronizeMode : unsigned { None, CPU, All } sync;
  enum class ExitReason : unsigned { UnknownEvent, FrameEvent, SynchronizeEvent };
  readonly<ExitReason> exit_reason;

  cothread_t host_thread;
  cothread_t active_thread;

  void enter();
  void exit(ExitReason);
  alwaysinline void swapto(Processor&);
  alwaysinline void synchronize_cpu();
  alwaysinline void synchronize_all();

  void power();
  Scheduler();
};

extern Scheduler scheduler;
