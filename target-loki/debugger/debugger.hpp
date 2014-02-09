struct Debugger {
  enum class Source : unsigned { CPU, SMP, PPU, DSP, APU, WRAM, VRAM, OAM, CGRAM };

  struct Breakpoint {
    Source source = Source::CPU;
    enum class Mode : unsigned { Disabled, Read, Write, Execute } mode = Mode::Disabled;
    unsigned addr = 0;
    maybe<uint8> data;
    unsigned triggered = 0;  //counter for number of times breakpoint was hit
  };

  struct Usage {
    enum : unsigned {
      Read     = 0x01,
      Write    = 0x02,
      Execute  = 0x04,
      //CPU
      FlagE    = 0x08,
      FlagM    = 0x10,
      FlagX    = 0x20,
      //APU
      FlagP    = 0x08,
      DspRead  = 0x10,
      DspWrite = 0x20,
    };
  };

  Debugger();

  void load();
  void unload();
  void main();

  void run();
  void stop();
  void leave();

  bool breakpointTest(Source source, Breakpoint::Mode mode, unsigned addr, uint8 data = 0x00);
  string cpuDisassemble();
  string cpuDisassemble(unsigned addr, bool e, bool m, bool x);
  void cpuExec(uint24 addr);
  void cpuRead(uint24 addr, uint8 data);
  void cpuWrite(uint24 addr, uint8 data);
  void echoBreakpoints();
  void echoDisassemble(Source source, unsigned addr, signed size);
  void echoHex(Source source, unsigned addr, signed size);
  void memoryExport(Source source, string filename);
  uint8 memoryRead(Source source, unsigned addr);
  unsigned memorySize(Source source);
  void memoryWrite(Source source, unsigned addr, uint8 data);
  void ppuCgramRead(uint16 addr, uint8 data);
  void ppuCgramWrite(uint16 addr, uint8 data);
  void ppuOamRead(uint16 addr, uint8 data);
  void ppuOamWrite(uint16 addr, uint8 data);
  void ppuVramRead(uint16 addr, uint8 data);
  void ppuVramWrite(uint16 addr, uint8 data);
  string smpDisassemble();
  string smpDisassemble(uint16 addr, bool p);
  void smpExec(uint16 addr);
  void smpRead(uint16 addr, uint8 data);
  void smpWrite(uint16 addr, uint8 data);
  string sourceName(Source source);
  void tracerDisable(Source source);
  void tracerEnable(Source source, string filename);
  void tracerMaskDisable(Source source);
  void tracerMaskEnable(Source source);

  bool running = false;

  uint8* apuUsage = nullptr;
  vector<Breakpoint> breakpoints;
  unsigned cpuInstructionCounter = 0;
  maybe<unsigned> cpuRunFor;
  maybe<unsigned> cpuRunTo;
  maybe<unsigned> cpuStepFor;
  maybe<unsigned> cpuStepTo;
  file cpuTracerFile;
  bitvector cpuTracerMask;
  uint8* cpuUsage = nullptr;
  unsigned smpInstructionCounter = 0;
  maybe<unsigned> smpRunFor;
  maybe<unsigned> smpRunTo;
  maybe<unsigned> smpStepFor;
  maybe<unsigned> smpStepTo;
  file smpTracerFile;
  bitvector smpTracerMask;
};

extern Debugger* debugger;
