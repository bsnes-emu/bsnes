struct Debugger {
  struct Flags {
    bool paused;   //do not run emulation when true; set when breakpoint is hit
    bool step;     //break on the next instruction from any processor

    bool debugCPU;
    bool debugSMP;
  } flags;

  void run();
  void echo(const string &text);
  void resume();   //start running until breakpoint is reached
  void suspend();  //stop running as soon as possible
  void tracerEnable(bool);

  //S-CPU
  void cpu_op_exec(uint24 addr);
  void cpu_op_read(uint24 addr);
  void cpu_op_write(uint24 addr, uint8 data);

  Debugger();

  file fpTracer;

  template<typename... Args> void print(Args&&... args) {
    string text(std::forward<Args>(args)...);
    echo(text);
  }
};

extern Debugger *debugger;
