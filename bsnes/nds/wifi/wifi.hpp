
struct WIFI {
  void power();
  
  void bbTransfer();
  void bbTransferBit();
  
  void rfTransfer();
  void rfTransferBit();
  
  uint32 read(uint32 addr, uint32 size);
  void write(uint32 addr, uint32 size, uint32 data);
  
  uint1  powered;
  
  uint16 reg004;
  uint3  swMode, wepMode;
  
  uint8  macAddr[6];
  uint8  bssId[6];
  uint4  assocIdl;
  uint11 assocIdf;
  
  uint16 rxControl, wepControl;
  uint16 reg034, baseBandPower;
  
  uint16 rxBufBegin, rxBufEnd;
  uint12 rxWritePos, rxWritePosLatch;
  uint12 rxReadPos,  rxSoftReadPos;
  uint12 rxGapBegin, rxGapSize;
  uint12 rxReadCounter;
  uint16 reg00a;
  
  uint12 txWritePos;
  uint12 txGapBegin, txGapSize;
  uint16 txBeacon;
  uint16 txMultiCmd;
  uint16 txSlots[3];
  uint8  txTimOffset;
  uint12 txWriteCounter;
  uint16 txStatControl;
  uint16 txRetryLimit;
  
  uint8  listenCount;
  uint8  listenInterval;
  uint10 beaconInterval;
  uint2  preamble;
  uint16 preBeacon;
  uint11 random;
  
  struct {
    uint1  enable;
    uint1  enableIrq;
    uint1  enableTxMP;
    uint64 count;
    uint64 compare;
  } timer;
  
  uint2  config0d4;
  uint12 config0d8;
  uint16 config0da;
  uint5  config0ecl;
  uint6  config0ech;
  
  uint9  config120l;
  uint1  config120h;
  uint16 config122;
  uint16 config124;
  uint16 config128;
  uint12 config130;
  uint12 config132l;
  uint1  config132h;
  uint16 configBeaconCount;
  
  uint16 config140;
  uint16 config142;
  uint8  config144;
  uint8  config146;
  uint8  config148;
  uint8  config14a;
  uint16 config14c;
  
  uint6  config150l;
  uint8  config150h;
  uint7  config154l;
  uint1  config154m;
  uint4  config154h;
  
  uint2  reg1a0l;
  uint2  reg1a0m;
  uint1  reg1a0n;
  uint1  reg1a0h;
  uint2  reg1a2;
  uint16 reg1a4;
  
  struct {
    uint16 enable, counterEnable, oflowEnable;
    uint16 flags,  counterFlags,  oflowFlags;
  } interrupt;
  
  struct {
    uint8  reg1b0;
    uint16 reg1b2;
    uint16 reg1b4;
    uint16 reg1b6;
    uint8  reg1b8;
    uint8  reg1ba;
    uint16 reg1bc;
    uint16 reg1be;
    uint16 txErrors;
    uint16 rxPackets;
    uint8  mpErrors[16];
  } stats;
  
  struct {
    uint4 txIdle;
    uint2 wakeRequest;
    uint1 wakePending;
    uint1 sleeping;
  } pm;
  
  // Baseband serial interface
  struct {
    uint16 header;       // 01rw0000 aaaaaaaa (r/w, address)
    uint8  read, write;  // data sent and received
    uint4  powerl;
    uint1  powerh;
    uint1  mode8, modeE;
    uint1  busy;
    uint1  clock;
    
    uint8  time;
    Event  event;
    
    uint8  regs[0x69];
  } bb;
  
  // RF serial interface
  struct {
    uint32 data;      // r aaaaa dd dddddddd dddddddd (r/w, address, data)
    uint6  length;    // should be 24 bits
    uint1  type;      // should be 0
    uint1  reserved;
    uint1  busy;
    
    uint8  time;
    Event  event;
    
    uint18 regs[0x20];
  } rf;
};

extern WIFI wifi;