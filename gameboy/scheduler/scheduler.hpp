struct Scheduler {
  cothread_t host_thread;
  cothread_t active_thread;

  void enter();
  void exit();
  void swapto(Processor&);

  void init();
  Scheduler();
};

extern Scheduler scheduler;
