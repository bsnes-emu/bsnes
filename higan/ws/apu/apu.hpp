struct APU : Thread, IO {
  static auto Enter() -> void;
  auto main() -> void;
  auto dacRun() -> void;
  auto step(uint clocks) -> void;
  auto power() -> void;

  //io.cpp
  auto portRead(uint16 addr) -> uint8;
  auto portWrite(uint16 addr, uint8 data) -> void;

  struct State {
    uint13 clock;
  } s;

  struct Registers {
    //$008f  SND_WAVE_BASE
    uint8 waveBase;

    //$0091  SND_OUTPUT
    uint1 speakerEnable;
    uint2 speakerShift;
    uint1 headphoneEnable;
  } r;

  struct Channel {
    Channel(uint id);
    auto sample(uint5 index) -> uint4;

    const uint id;

    struct Output {
      int16 left;
      int16 right;
    } o;
  };

  struct Channel0 : Channel {
    Channel0();
    auto run() -> void;

    struct State {
      uint11 period;
      uint5 sampleOffset;
    } s;

    struct Registers {
      //$0080-0081  SND_CH0_PITCH
      uint11 pitch;

      //$0088  SND_CH0_VOL
      uint4 volumeLeft;
      uint4 volumeRight;

      //$0090  SND_CTRL
      uint1 enable;
    } r;
  } channel0;

  struct Channel1 : Channel {
    Channel1();
    auto run() -> void;

    struct State {
      uint11 period;
      uint5 sampleOffset;
    } s;

    struct Registers {
      //$0082-0083  SND_CH1_PITCH
      uint11 pitch;

      //$0089  SND_CH1_VOL
      uint4 volumeLeft;
      uint4 volumeRight;

      //$0090  SND_CTRL
      uint1 enable;
      uint1 voice;

      //$0092  SND_VOICE_CTRL
      uint2 voiceEnableLeft;
      uint2 voiceEnableRight;
    } r;
  } channel1;

  struct Channel2 : Channel {
    Channel2();
    auto sweep() -> void;
    auto run() -> void;

    struct State {
      uint11 period;
      uint5 sampleOffset;

      int sweepCounter;
    } s;

    struct Registers {
      //$0084-0085  SND_CH2_PITCH
      uint11 pitch;

      //$008a  SND_CH2_VOL
      uint4 volumeLeft;
      uint4 volumeRight;

      //$008c  SND_SWEEP_VALUE
      uint8 sweepValue;

      //$008d  SND_SWEEP_TIME
      uint5 sweepTime;

      //$0090  SND_CTRL
      uint1 enable;
      uint1 sweep;
    } r;
  } channel2;

  struct Channel3 : Channel {
    Channel3();
    auto noiseSample() -> uint4;
    auto run() -> void;

    struct State {
      uint11 period;
      uint5 sampleOffset;

      uint1 noiseOutput;
      uint15 noiseLFSR;
    } s;

    struct Registers {
      //$0086-0087  SND_CH3_PITCH
      uint11 pitch;

      //$008b  SND_CH3_VOL
      uint4 volumeLeft;
      uint4 volumeRight;

      //$008e  SND_NOISE
      uint3 noiseMode;
      uint1 noiseReset;
      uint1 noiseUpdate;

      //$0090  SND_CTRL
      uint1 enable;
      uint1 noise;
    } r;
  } channel3;

  struct Channel4 : Channel {
    Channel4();
    auto run() -> void;

    struct State {
      int8 data;
    } s;

    struct Registers {
      //$006a  HYPER_CTRL
      uint2 volume;
      uint2 scale;
      uint3 speed;
      uint1 enable;

      //$006b  HYPER_CHAN_CTRL
      uint4 unknown;
      uint1 leftEnable;
      uint1 rightEnable;
    } r;
  } channel4;
};

extern APU apu;
