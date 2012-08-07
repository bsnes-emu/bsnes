
struct GameCard;
struct SPIDevice;

// Pins 1-17:  GND, Clock, -, /ROM, /RESET, /SPI, /IRQ, VCC, D0-D7, GND
// 
// This is a hybrid serial-parallel bus, one byte transferred each clock.
// Both the cards and the slot support XOR obfuscation using a 39-bit LFSR,
// and some commands are further encrypted via a Blowfish variant.
//
// Transfer rates are 33MHz /5 or /8, which is 4-6MB/sec, and there's a
// 32-bit buffer making it possible to DMA straight to VRAM. For perspective,
// 6MB/s is enough to stream 256x192x16bpp video + audio at 60fps.

struct Slot1 {
  Slot1();
  void power();
  void load(GameCard* card);
  GameCard* unload();
  
  void startRomTransfer();
  uint8 readRom();
  uint8 spiTransfer(uint8 data);
  
  GameCard *card;
  
  uint1  enable;
  
  // ROM interface
  uint1  clock;           // 33MHz / {5, 8}
  uint13 decryptLatency;  // clocks to wait for card to decrypt command
  uint6  responseLatency; // clocks to wait for response (data buffering?)
  uint2  xorData;         // XOR data received (2 bits?)
  uint1  xorCmds;         // XOR commands sent
  uint1  dataReady;       // 32 bits buffered and ready in read port
  uint3  blockSize;       // {0, 512 bytes (usual), 1K-16K, 32 bits}
  uint1  secureMode;      // ?
  uint1  transferIrq;     // generate IRQ at end of block
  uint1  transferPending; // still more bytes to transfer?
  uint32 transferLength;  // # bytes remaining
  
  uint64 command;         // latch holding 8 command bytes for next transfer
  uint64 lfsr[2];         // registers used to obfuscate communication
  
  // Serial interface for EEPROM, flash, and peripheral access.
  // Each transfer writes and reads 8 bits, one bit per clock.
  // Pins D6/D7 do double duty as data in/out respectively.
  struct SPI {
    uint8 data;  // read from last transfer
    uint2 baud;  // 4MHz >> n
    uint1 enable, busy, hold;
  } spi;
};

struct GameCard {
  virtual ~GameCard();
  GameCard(uint32 id);
  GameCard(const stream& memory, uint32 size, uint32 id);
  
  virtual void power();
  virtual void command(uint64 command);
  virtual uint8 read();
  
  int    state;    enum { idle, readData, readId };
  uint32 block;    // 4K block for reading
  uint32 offset;   // byte offset
  uint32 size;     // power of 2 rom size
  uint64 lfsr[2];  // for obfuscation
  uint32 chipId;
  string sha256;
  
  StaticMemory rom;
  SPIDevice    *spi;
};

struct IRPort : SPIDevice {
  IRPort() : slave(nullptr) { }
  IRPort(SPIDevice *slave) : slave(slave) { }
  
  void power();
  void select(bool state);
  uint8 transfer(uint8 data);
  
  SPIDevice *slave;
  bool bypass;
  uint8 command;
};

struct S1EEPROM : SPIDevice, StaticMemory {
  void select(bool state);
  uint8 transfer(uint8 data);
  
  int    state;    enum { idle, params, dataIo };
  uint8  command;  enum {
      cmdWriteDisable, cmdWriteEnable,  // 0x04, 0x06
      cmdWriteStatus,  cmdReadStatus,   // 0x01, 0x05
      cmdWrite,        cmdRead,         // 0x02, 0x03
      cmdReadID,                        // 0x9f
    };
  uint8  numArgs;
  uint32 offset;
  
  // Status register
  uint1  busy;
  uint1  writeEnable;
  uint2  writeProtect;
  uint1  statusLock;
  
  uint1  unlockable;  // /W pin, presumably enabled on game cards
};


extern Slot1 slot1;
