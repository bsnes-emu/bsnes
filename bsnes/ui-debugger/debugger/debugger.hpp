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
      bool nmi;
      bool irq;
    } cpu;
    struct SMP {
      bool stepInto;
    } smp;
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

  //S-CPU
  void cpu_op_exec(uint24 addr);
  void cpu_op_read(uint24 addr);
  void cpu_op_write(uint24 addr, uint8 data);
  void cpu_op_nmi();
  void cpu_op_irq();

  //S-SMP
  void smp_op_exec(uint16 addr);
  void smp_op_read(uint16 addr);
  void smp_op_write(uint16 addr, uint8 data);

  //S-PPU
  void ppu_vram_read(uint16 addr);
  void ppu_oam_read(uint16 addr);
  void ppu_cgram_read(uint16 addr);
  void ppu_vram_write(uint16 addr, uint8 data);
  void ppu_oam_write(uint16 addr, uint8 data);
  void ppu_cgram_write(uint16 addr, uint8 data);

  Debugger();

  template<typename... Args> void print(Args&&... args) {
    string text(std::forward<Args>(args)...);
    echo(text);
  }
};

extern Debugger *debugger;
