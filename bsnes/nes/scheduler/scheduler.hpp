struct Scheduler {
  cothread_t host_thread;  //program thread (used to exit emulation)
  cothread_t thread;       //active emulation thread (used to enter emulation)

  void enter();
  void exit();

  void power();
  void reset();
};

extern Scheduler scheduler;
