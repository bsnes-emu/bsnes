#include "eventqueue.hpp"

typedef function<void()> Action;
typedef EventQueue_of<Action> EventQueue;
typedef EventQueue_of<Action>::Elem Event;


enum class Input : unsigned {
  // REG_KEYINPUT (10 bits)
  A, B, Select, Start, Right, Left, Up, Down, R, L,
  
  // REG_AUXINPUT (8 bits)
  // - C, Z, W seem present, but unconnected
  // - D is supposedly on debug units
  X, Y, C, D, Z, W, Pen, Lid,
  
  // Touchscreen
  PenX, PenY, PenZ1, PenZ2,
};


struct SPIDevice {
  virtual ~SPIDevice();
  virtual void power() = 0;
  virtual void select(bool state) = 0;
  virtual uint8 transfer(uint8 data) = 0;
};

#include "../memory/eeprom.hpp"
#include "../memory/fram.hpp"
#include "../memory/flash.hpp"

struct PowerMgr : SPIDevice {
  ~PowerMgr();
  void power();
  void select(bool state);
  uint8 transfer(uint8 data);
};


struct Touchscreen : SPIDevice {
  ~Touchscreen();
  void power();
  void select(bool state);
  uint8 transfer(uint8 data);
  
  bool penDown();
  
  uint1 bitDepth;   enum { read12, read8 };
  uint1 refMode;    enum { diff, single };
  uint2 powerMode;  enum { powerAuto, powerADC, powerRef, powerFull };
  uint3 input;      enum {
      temp0     = 0, temp1     = 7,
      vbatt     = 2, aux       = 6, 
      ypos      = 1, xpos      = 5,
      pressure0 = 3, pressure1 = 4,
    };
  
  uint32 adc; 
  uint12 last;
};


struct Clock {
  Clock();
  ~Clock();
  void reset();
  void power();
  uint4 io(uint4 pins);
  
  void dateTime();
  void alarmTime(unsigned no);
  void field(uint8& src, uint8 mask);
  
  void tick();
  void tickSecond();
  void tickDay();
  bool incBCD(uint8& val, uint8 first, uint8 last);
  
  void freeze(int64 curSec, int64 curUSec);
  void thaw(int64 curSec, int64 curUSec);
  
  
  int64 stoppedSec, stoppedUSec;
  
  uint1 lastCs, lastClk;
  uint8 buffer, dataOut;
  uint3 nbits;
  uint8 command, index;
  bool  output;
  
  uint8 status1;  enum {
    resetBit = 0x01, hourMode  = 0x02,
    int1Flag = 0x10, int2Flag  = 0x20,
    powerLow = 0x40, powerLost = 0x80
  };
  uint8 status2;  enum {
    int1Mode = 0x0f,
    int2Mode = 0x40, testMode  = 0x80
  };
  uint8 adjust;  // clock rate fine adjustment
  uint8 userByte;
  
  Event event;
  uint16 counter;  // 32KHz subsecond counter
  uint1 intr[2];   // Interrupts (both go to IRQ pin)
  uint1 hold;      // Delays seconds tick while reading the time
  uint16 lag;      // How long the seconds tick was delayed
  uint1 secondsTickPending;
  
  union {
    struct { uint8 year, month, day, weekday, hour, minute, second; };
    struct { uint8 time[7]; };
  };
  union {
    struct { uint8 weekday,  hour,  minute; };
    struct { uint8 setting[3];              };
  } alarm[2];
};


struct VRAMMapping : HalfMemory {
  VRAMMapping();
  VRAMMapping(HalfMemory& target, uint8* dirtyBits, unsigned pageno);
  ~VRAMMapping();
  
  uint16& operator[](uint32 addr);
  uint32 read(uint32 addr, uint32 size);
  void write(uint32 addr, uint32 size, uint32 word);
  bool dirty(uint32 addr);
  void clearDirty(uint32 addr, uint32 size);
  
  uint8 *dirtyBits;
};


struct System {
  cothread_t  callerThread;
  cothread_t  activeThread;
  bool        running;
  
  Flash       firmware;
  Touchscreen touchscreen;
  PowerMgr    powerMgr;
  Clock       clock;
  
  SDRAM      ewram;      // 4MB shared external WRAM
  SRAM       iwram;      // 64KB 32-bit work RAM, private to ARM7
  SRAM       swram[2];   // 16KB x 2 banks switchable 32-bit WRAM
  HalfMemory vram[9];    // 656KB in 9 banks of 16-bit VRAM
  HalfMemory wxram;      // 8KB wireless packet buffer
  
  uint16 unmappedVram[0x2000];
  
  struct VRAMConfig {
    uint1 enable;
    uint2 offset;
    uint3 bus;  // aka. MST or "Magic Selector Thing"
      enum {
        display=0, // all banks
        bg0=1,     // A,B,C,D,E,F,G
        obj0=2,    // A,B,    E,F,G
        arm7=2,    //     C,D
        tex=3,     // A,B,C,D
        texpal=3,  //         E,F,G
        bgpal0=4,  //         E,F,G
        objpal0=5, //        E?,F,G
        ppu1=4,    //     C,D
        bg1=1,     //               H,I
        obj1=2,    //                 I
        bgpal1=2,  //               H
        objpal1=3, //                 I
      };
    
    operator uint8() {
      return enable<<7 | offset<<3 | bus<<0;
    }
    VRAMConfig& operator=(uint8 data) {
      enable = data>>7;
      offset = data>>3;
      bus    = data>>0;
      return *this;
    }
  };
  
  struct {
    // Mappings for each graphics bus that can have VRAM banked onto it.
    // Each page is 16K, the finest granularity.
    VRAMMapping arm9[8][64];  // 8x1M arm9 map - 0x060,062,064,066,068,..
    VRAMMapping arm7[16];     // 256K arm7 map - 0x060..
    
    // Locked memory can be used by the GPU, but isn't mapped in CPU space.
    VRAMMapping tex[32];      //   512K locked texture memory
    VRAMMapping texpal[8];    //   128K locked texture palettes
    VRAMMapping bgpal[2][8];  //    32K locked BG palettes
    VRAMMapping objpal[2][8]; //     8K locked OBJ palettes
    
    VRAMMapping &bg(int w, int i)  { return arm9[0+w][i]; }
    VRAMMapping &obj(int w, int i) { return arm9[2+w][i]; }
    VRAMMapping &display(int i)    { return arm9[4][i&31]; }
    
    VRAMConfig  regs[10];     // A,B,C,D, E,F,G,SWRAM, H,I
    
    uint8 dirty[(512 + 96 + 48)*1024 / 256];
  } vmap;
  
  
  void clearVmap();
  void updateVmap();
  void mapVram(VRAMMapping* loc, uint8 *dirtyBits, unsigned npages, HalfMemory& bank);
  
  uint32 regVmap(unsigned index);
  void regVmap(unsigned index, uint32 data, uint32 mask);
  
  void loadArm7Bios(const stream&);
  void loadArm9Bios(const stream&);
  void loadFirmware(const stream&);
  void loadRTC(const stream&);
  
  void saveFirmware(const stream&);
  void saveRTC(const stream&);
  
  void power();
  void run();
  
  void frame();
  
  System();
};

extern System system;
