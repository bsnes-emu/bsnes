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
    uint9 address;
  } io;

  struct LFO {
    uint1  enable;
    uint3  rate;
    uint32 clock;
    uint32 divider;
  } lfo;

  struct DAC {
    uint1 enable;
    uint8 sample;
  } dac;

  struct Envelope {
    uint32 clock;
    uint32 divider;
  } envelope;

  struct TimerA {
    //timer.cpp
    auto run() -> void;
    auto power() -> void;

    uint1  enable;
    uint1  irq;
    uint1  line;
    uint10 period;
    uint10 counter;
  } timerA;

  struct TimerB {
    //timer.cpp
    auto run() -> void;
    auto power() -> void;

    uint1 enable;
    uint1 irq;
    uint1 line;
    uint8 period;
    uint8 counter;
    uint4 divider;
  } timerB;

  enum : uint { Attack, Decay, Sustain, Release };

  struct Channel {
    //channel.cpp
    auto power() -> void;

    bool  leftEnable;
    bool  rightEnable;

    uint3 algorithm;
    uint3 feedback;
    uint3 vibrato;
    uint2 tremolo;

    uint2  mode;

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

      bool   keyOn;
      bool   lfoEnable;
      uint3  detune;
      uint4  multiple;
      uint7  totalLevel;

      uint16 outputLevel;
      int16  output;
      int16  prior;

      struct Pitch {
        uint11 value;
        uint11 reload;
        uint11 latch;
      } pitch;

      struct Octave {
        uint3 value;
        uint3 reload;
        uint3 latch;
      } octave;

      struct Phase {
        uint20 value;
        uint20 delta;
      } phase;

      struct Envelope {
        uint   state;
        int    rate;
        int    divider;
        uint32 steps;
        uint10 value;

        uint2  keyScale;
        uint5  attackRate;
        uint5  decayRate;
        uint5  sustainRate;
        uint4  sustainLevel;
        uint5  releaseRate;
      } envelope;

      struct SSG {
        bool enable;
        bool attack;
        bool alternate;
        bool hold;
        bool invert;
      } ssg;
    } operators[4]{*this, *this, *this, *this};

    auto operator[](uint2 index) -> Operator& { return operators[index]; }
  } channels[6];

  uint16 sine[0x400];
  int16  pow2[0x200];

  int lpfLeft;
  int lpfRight;

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
