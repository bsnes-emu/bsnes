struct APU : Thread {
  shared_pointer<Emulator::Stream> stream;

  APU();

  static auto Enter() -> void;
  auto main() -> void;
  auto tick() -> void;
  auto setIRQ() -> void;
  auto setSample(int16 sample) -> void;

  auto power() -> void;

  auto readIO(uint16 addr) -> uint8;
  auto writeIO(uint16 addr, uint8 data) -> void;

  auto serialize(serializer&) -> void;

  struct Filter {
    auto runHipassStrong(int sample) -> int;
    auto runHipassWeak(int sample) -> int;
    auto runLopass(int sample) -> int;

    auto serialize(serializer&) -> void;

    enum : int { HiPassStrong = 225574, HiPassWeak = 57593, LoPass = 86322413 };

    int64 hipassStrong;
    int64 hipassWeak;
    int64 lopass;
  };

  struct Envelope {
    auto volume() const -> uint;
    auto clock() -> void;

    auto power() -> void;

    auto serialize(serializer&) -> void;

    uint4 speed;
    bool useSpeedAsVolume;
    bool loopMode;

    bool reloadDecay;
    uint8 decayCounter;
    uint4 decayVolume;
  };

  struct Sweep {
    auto checkPeriod() -> bool;
    auto clock(uint channel) -> void;

    auto power() -> void;

    auto serialize(serializer&) -> void;

    uint8 shift;
    bool decrement;
    uint3 period;
    uint8 counter;
    bool enable;
    bool reload;
    uint11 pulsePeriod;
  };

  struct Pulse {
    auto clockLength() -> void;
    auto checkPeriod() -> bool;
    auto clock() -> uint8;

    auto power() -> void;

    auto serialize(serializer&) -> void;

    uint lengthCounter;

    Envelope envelope;
    Sweep sweep;

    uint2 duty;
    uint3 dutyCounter;

    uint11 period;
    uint periodCounter;
  } pulse[2];

  struct Triangle {
    auto clockLength() -> void;
    auto clockLinearLength() -> void;
    auto clock() -> uint8;

    auto power() -> void;

    auto serialize(serializer&) -> void;

    uint lengthCounter;

    uint8 linearLength;
    bool haltLengthCounter;

    uint11 period;
    uint periodCounter;

    uint5 stepCounter;
    uint8 linearLengthCounter;
    bool reloadLinear;
  } triangle;

  struct Noise {
    auto clockLength() -> void;
    auto clock() -> uint8;

    auto power() -> void;

    auto serialize(serializer&) -> void;

    uint lengthCounter;

    Envelope envelope;

    uint4 period;
    uint periodCounter;

    bool shortMode;
    uint15 lfsr;
  } noise;

  struct DMC {
    auto start() -> void;
    auto stop() -> void;
    auto clock() -> uint8;

    auto power() -> void;

    auto serialize(serializer&) -> void;

    uint lengthCounter;
    bool irqPending;

    uint4 period;
    uint periodCounter;

    bool irqEnable;
    bool loopMode;

    uint8 dacLatch;
    uint8 addrLatch;
    uint8 lengthLatch;

    uint15 readAddr;
    uint dmaDelayCounter;

    uint3 bitCounter;
    bool dmaBufferValid;
    uint8 dmaBuffer;

    bool sampleValid;
    uint8 sample;
  } dmc;

  struct FrameCounter {
    auto serialize(serializer&) -> void;

    enum : uint { NtscPeriod = 14915 };  //~(21.477MHz / 6 / 240hz)

    bool irqPending;

    uint2 mode;
    uint2 counter;
    int divider;
  };

  auto clockFrameCounter() -> void;
  auto clockFrameCounterDivider() -> void;

  Filter filter;
  FrameCounter frame;

  uint8 enabledChannels;
  int16 cartridgeSample;

  int16 pulseDAC[32];
  int16 dmcTriangleNoiseDAC[128][16][16];

  static const uint8 lengthCounterTable[32];
  static const uint16 dmcPeriodTableNTSC[16];
  static const uint16 dmcPeriodTablePAL[16];
  static const uint16 noisePeriodTableNTSC[16];
  static const uint16 noisePeriodTablePAL[16];
};

extern APU apu;
