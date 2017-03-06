//Yamaha YM2612

struct YM2612 : Thread {
  shared_pointer<Emulator::Stream> stream;

  static auto Enter() -> void;
  auto main() -> void;
  auto sample() -> void;
  auto step(uint clocks) -> void;

  auto power() -> void;

  //io.cpp
  auto readStatus() -> uint8;
  auto writeAddress(uint9 data) -> void;
  auto writeData(uint8 data) -> void;

private:
  struct IO {
    uint9 address = 0;
  } io;

  struct LFO {
    uint1  enable = 0;
    uint3  rate = 0;
    uint32 clock = 0;
    uint32 divider = 0;
  } lfo;

  struct DAC {
    uint1 enable = 0;
    uint8 sample = 0;
  } dac;

  struct Envelope {
    uint32 clock = 0;
    uint32 divider = 0;
  } envelope;

  struct TimerA {
    //timer.cpp
    auto run() -> void;

    uint1  enable = 0;
    uint1  irq = 0;
    uint1  line = 0;
    uint10 period = 0;
    uint10 counter = 0;
  } timerA;

  struct TimerB {
    //timer.cpp
    auto run() -> void;

    uint1 enable = 0;
    uint1 irq = 0;
    uint1 line = 0;
    uint8 period = 0;
    uint8 counter = 0;
    uint4 divider = 0;
  } timerB;

  enum : uint { Attack, Decay, Sustain, Release };

  struct Channel {
    //channel.cpp
    auto power() -> void;

    uint1 leftEnable = 1;
    uint1 rightEnable = 1;

    uint3 algorithm = 0;
    uint3 feedback = 0;
    uint3 vibrato = 0;
    uint2 tremolo = 0;

    uint2 mode = 0;

    struct Operator {
      Channel& channel;
      Operator(Channel& channel) : channel(channel) {}

      //channel.cpp
      auto trigger(bool) -> void;

      auto runEnvelope() -> void;
      auto runPhase() -> void;

      auto updateEnvelope() -> void;
      auto updatePitch() -> void;
      auto updatePhase() -> void;
      auto updateLevel() -> void;

      uint1 keyOn = 0;
      uint1 lfoEnable = 0;
      uint3 detune = 0;
      uint4 multiple = 0;
      uint7 totalLevel = 0;

      uint16 outputLevel = 0x1fff;
      int16  output = 0;
      int16  prior = 0;

      struct Pitch {
        uint11 value = 0;
        uint11 reload = 0;
        uint11 latch = 0;
      } pitch;

      struct Octave {
        uint3 value = 0;
        uint3 reload = 0;
        uint3 latch = 0;
      } octave;

      struct Phase {
        uint20 value = 0;
        uint20 delta = 0;
      } phase;

      struct Envelope {
        uint   state = Release;
        int    rate = 0;
        int    divider = 11;
        uint32 steps = 0;
        uint10 value = 0x3ff;

        uint2  keyScale = 0;
        uint5  attackRate = 0;
        uint5  decayRate = 0;
        uint5  sustainRate = 0;
        uint4  sustainLevel = 0;
        uint5  releaseRate = 1;
      } envelope;

      struct SSG {
        uint1 enable = 0;
        uint1 attack = 0;
        uint1 alternate = 0;
        uint1 hold = 0;
        uint1 invert = 0;
      } ssg;
    } operators[4]{*this, *this, *this, *this};

    auto operator[](uint2 index) -> Operator& { return operators[index]; }
  } channels[6];

  uint16 sine[0x400];
  int16  pow2[0x200];

  //constants.cpp
  struct EnvelopeRate {
    uint32_t divider;
    uint32_t steps[4];
  };

  static const uint8_t lfoDividers[8];
  static const uint8_t vibratos[8][16];
  static const uint8_t tremolos[4];
  static const uint8_t detunes[3][8];
  static const EnvelopeRate envelopeRates[16];
};

extern YM2612 ym2612;
