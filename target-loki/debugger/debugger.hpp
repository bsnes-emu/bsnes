struct Debugger {
  Debugger();

  void load();
  void unload();

  void run();
  void stop();

  string cpuDisassemble();
  string cpuDisassemble(unsigned addr, bool e, bool m, bool x);
  void cpuExec(uint24 addr);
  void cpuRead(uint24 addr, uint8 data);
  void cpuWrite(uint24 addr, uint8 data);
  void echoBreakpoints();
  void echoDisassemble(unsigned addr, signed size);
  void echoHex(unsigned addr, signed size);
  uint8_t read(unsigned addr);
  void write(unsigned addr, uint8_t data);

  enum class Mode : unsigned { Break, Run, Step } mode = Mode::Break;

  struct Breakpoint {
    enum class Mode : unsigned { Disabled, Read, Write, Execute } mode = Mode::Disabled;
    unsigned addr = 0;
    optional<uint8_t> data = false;
    unsigned triggered = 0;  //counter for number of times breakpoint was hit
  };

  struct Usage {
    enum : unsigned {
      Read    = 0x01,
      Write   = 0x02,
      Execute = 0x04,
      FlagE   = 0x08,
      FlagM   = 0x10,
      FlagX   = 0x20,
    };
  };

  vector<Breakpoint> breakpoints;
  optional<unsigned> runFor = false;
  optional<unsigned> runTo = false;
  unsigned stepDuration = 0;
  file tracerFile;
  bool tracerMask = false;
  uint8_t* usageCPU = nullptr;
};

extern Debugger* debugger;
