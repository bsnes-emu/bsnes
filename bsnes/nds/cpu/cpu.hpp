#include "core.hpp"

struct CPUCore : ARMCore {
  CPUCore();
  ~CPUCore();
  
  void power();
  void hdraw();
  void hblank();
  
  string tracePsr(uint32 value);
  void traceInsn();
  
  uint32 readReg(uint32 addr, uint32 size);
  void writeReg(uint32 addr, uint32 size, uint32 data);
  
  void popMsg();
  void pushMsg(uint32 data);
  void clearMsg();
  
  uint32 regMsgControl();
  uint32 regMsgRecv();
  uint32 regSync();
  
  void regMsgControl(uint32 data, uint32 mask);
  void regMsgSend(uint32 data);
  void regSync(uint32 data, uint32 mask);
  
  void dmaTransfer(unsigned no);
  bool dmaTrigger(unsigned channels, unsigned value);
  
  uint32 regDmaControl(unsigned no);
  uint32 regDmaSource(unsigned no);
  uint32 regDmaDest(unsigned no);
  uint32 regDmaFill(unsigned no);
  
  void regDmaControl(unsigned no, uint32 data, uint32 mask);
  void regDmaSource(unsigned no, uint32 data, uint32 mask);
  void regDmaDest(unsigned no, uint32 data, uint32 mask);
  void regDmaFill(unsigned no, uint32 data, uint32 mask);
  
  void updateTimer(unsigned no);
  uint32 regTimer(unsigned no);
  void regTimer(unsigned no, uint32 data, uint32 mask);
  
  uint32 regSlot1Control();
  uint32 regSlot1RomControl();
  uint32 regSlot1RomCommand(unsigned index);
  uint32 regSlot1RomRecv();
  
  void regSlot1Control(uint32 data, uint32 mask);
  void regSlot1RomControl(uint32 data, uint32 mask);
  void regSlot1RomCommand(unsigned index, uint32 data, uint32 mask);
  void regSlot1RomSeed(unsigned index, uint32 data, uint32 mask);
  
  uint32 regSlot2Control();
  void regSlot2Control(uint32 data, uint32 mask);
  
  enum {
    // REG_IE, REG_IF bits
    irqVBlank   = 1<<0,    irqHBlank   = 1<<1,
    irqVMatch   = 1<<2,    irqTimer    = 1<<3,
    irqClock    = 1<<7,    irqDma      = 1<<8,
    irqKeypad   = 1<<12,   irqSlot2    = 1<<13,
    
    irqSync     = 1<<16,
    irqMsgSent  = 1<<17,   irqMsgRecv  = 1<<18,
    irqCardDone = 1<<19,   irqSlot1    = 1<<20,
    irqGeomBuf  = 1<<21,   irqLid      = 1<<22,
    irqSpi      = 1<<23,   irqWifi     = 1<<24,
  };
  
  struct {
    bool   arm7, arm9;
    
    uint64 xorSeeds[2];
    
    uint2  slot2ramTiming;
    uint2  slot2romTiming0;
    uint1  slot2romTiming1;
    uint2  slot2phi;
  } config;
  
  struct {
    bool   gate;
    uint32 enable, flags;
  } interrupt;
  
  struct {
    uint1  enable, irqOnRecv, irqOnSent;
    uint1  error, empty, full;
    uint4  writeOffset, readOffset;
    uint32 buffer[16];
  } msg;
  
  struct {
    uint1  enable;
    uint4  output;
  } sync;
  
  struct {
    uint1  inVBlank, inHBlank, inVMatch; // vblank, hblank, line?
    uint1  irqOnVBlank, irqOnHBlank, irqOnVMatch; // virq, hirq, lirq?
    uint9  vcompare; // lineCompare?
  } status;
  
  struct {
    uint1  enable, irq, size, repeat;
    uint3  trigger;
    uint2  srcStep, destStep;
    uint32 source, dest, count, fill;
    struct {
      uint32 source, dest, count;
    } init;
  } dma[4];
  
  struct Timer {
    uint1  enable, irq, cascade;
    uint2  divider;
    uint16 reload, count;
    uint32 lastUpdate;
    
    Event  event;
    
    uint32 regRead();
    void regWrite(uint32 data, uint32 mask);
    void update();
    void operator()();
  } timer[4];
  
  struct Ev {
    bool   irq;
    bool   anydma;
    
    EventQueue queue;
    
    Ev() : queue(60) {}
  } event;
  
  bool     trace;
  uint32   insnLatch;
  unsigned powerState;
    enum { running, waiting, sleeping };
  
  WordMemory bios;
  
  CPUCore* other;
  
  cothread_t thread;
  int32 clock;
};



struct ARM7TDMI : CPUCore {
  static void Thread();
  void main();
  void power();
  void step(unsigned clocks);
  
  alwaysinline void istep(unsigned clocks);
  alwaysinline void execARM();
  alwaysinline void execTHUMB();
  
  uint32 fetch(uint32 addr, uint32 size, bool s);
  uint32 read(uint32 addr, uint32 size, bool s);
  void write(uint32 addr, uint32 size, bool s, uint32 data);
  void dataCop(uint4 cpno, uint4 op1, uint4 ird, uint4 irn, uint4 irm, uint4 op2);
  
  uint32 readReg(uint32 addr, uint32 size);
  void writeReg(uint32 addr, uint32 size, uint32 data);
  
  ARM7TDMI();
  
  struct SPI {
    uint1 enable, hold, irq, size;
    uint2 divider; // 4, 2, 1, 0.5 MHz
    uint2 device;  enum { power, flash, touch, none };
    uint16 data;
  } spi;
  
  struct RTC {
    uint4 in[2];
    uint4 out[2];
    uint4 dir[2];
    uint8 buffer;
    uint3 index;
  } rtc;
  
  struct SIO {
    uint4 in, out, dir;
    uint1 irq;
    uint2 mode;
  } sio;
  
  uint1 booted;
  uint1 flag300;
  
  uint32 regSpi();
  void regSpi(uint32 data, uint32 mask);

  uint32 regRtc();
  void regRtc(uint32 data, uint32 mask);
  
  uint32 regSio();
  void regSio(uint32 data, uint32 mask);
};



struct ARM946ES : CPUCore {
  SRAM   itcm;
  SRAM   dtcm;
  uint32 itcmRegion, itcmRCompare, itcmRMask, itcmWCompare, itcmWMask;
  uint32 dtcmRegion, dtcmRCompare, dtcmRMask, dtcmWCompare, dtcmWMask;
  
  struct CR {
    uint1 mmu;
    uint1 dcache, icache;
    uint1 endian;           enum { little, big };
    uint1 cachePolicy;      enum { random, roundRobin };
    uint1 dtcm, dtcmLoad;
    uint1 itcm, itcmLoad;
  } control;
  
  // Math functions
  uint2  divMode;   enum { div32, div64_32, div64 };
  uint1  divByZero;
  uint1  divBusy;
  int64  numerator;
  int64  denominator;
  int64  quotient;
  int64  remainder;
  
  uint1  rootMode;  enum { sqrt32, sqrt64 };
  uint1  rootBusy;
  uint64 square;
  uint32 squareRoot;
  
  uint1  booted;
  uint1  flag300;
  uint1  slot1access;
  uint1  slot2access;
  uint1  ramPriority;
  
  static void Thread();
  void main();
  void power();
  void step(unsigned clocks);
  
  alwaysinline void istep(unsigned clocks);
  alwaysinline void execARM();
  alwaysinline void execTHUMB();
  
  uint32 fetch(uint32 addr, uint32 size, bool s);
  uint32 read(uint32 addr, uint32 size, bool s);
  void write(uint32 addr, uint32 size, bool s, uint32 data);
  void dataCop(uint4 cpno, uint4 op1, uint4 ird, uint4 irn, uint4 irm, uint4 op2);
  
  uint32 readReg(uint32 addr, uint32 size);
  void writeReg(uint32 addr, uint32 size, uint32 data);
  
  void updateTcm();
  
  void regDivideControl(uint32 data, uint32 mask);
  void regNumerator(unsigned index, uint32 data, uint32 mask);
  void regDenominator(unsigned index, uint32 data, uint32 mask);
  void regSquareRootControl(uint32 data, uint32 mask);
  void regSquare(unsigned index, uint32 data, uint32 mask);
  
  uint32 regDivideControl();
  uint32 regNumerator(unsigned index);
  uint32 regDenominator(unsigned index);
  uint32 regQuotient(unsigned index);
  uint32 regRemainder(unsigned index);
  uint32 regSquareRootControl();
  uint32 regSquare(unsigned index);
  uint32 regSquareRoot();
  
  void startDivide();
  void startSquareRoot();
  
  ARM946ES();
};


extern ARM7TDMI arm7;
extern ARM946ES arm9;
