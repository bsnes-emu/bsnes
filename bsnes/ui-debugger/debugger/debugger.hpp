struct Usage {
  enum : unsigned { Read = 4, Write = 2, Exec = 1 };
  uint8_t *data;
  unsigned size;

  void allocate(unsigned size);
  void reset();
  Usage();
  ~Usage();
};

struct Debugger {
  bool paused;

  struct Flags {
    bool step;
    struct CPU {
      bool stepInto;
    } cpu;
  } flags;

  struct Debug {
    bool cpu;
    bool smp;
  } debug;

  Usage cpuUsage;
  Usage apuUsage;
  void loadUsage();
  void saveUsage();
  void resetUsage();

  void run();
  void echo(const string &text);
  void resume();   //start running until breakpoint is reached
  void suspend();  //stop running as soon as possible
  void tracerEnable(bool);

  //S-CPU
  void cpu_op_exec(uint24 addr);
  void cpu_op_read(uint24 addr);
  void cpu_op_write(uint24 addr, uint8 data);

  //S-SMP
  void smp_op_exec(uint16 addr);
  void smp_op_read(uint16 addr);
  void smp_op_write(uint16 addr, uint8 data);

  Debugger();

  file fpTracer;

  template<typename... Args> void print(Args&&... args) {
    string text(std::forward<Args>(args)...);
    echo(text);
  }
};

extern Debugger *debugger;
