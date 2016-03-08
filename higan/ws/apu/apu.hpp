struct APU : Thread, IO {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;
  auto power() -> void;

  //io.cpp
  auto portRead(uint16 addr) -> uint8;
  auto portWrite(uint16 addr, uint8 data) -> void;

  struct Channel1 {
    struct Registers {
      //$0080-0081  SND_CH1_PITCH
      uint11 pitch;

      //$0088  SND_CH1_VOL
      uint4 volumeLeft;
      uint4 volumeRight;

      //$0090  SND_CTRL
      uint1 enable;
    } r;
  } channel1;

  struct Channel2 {
    struct Registers {
      //$0082-0083  SND_CH2_PITCH
      uint11 pitch;

      //$0089  SND_CH2_VOL
      uint4 volumeLeft;
      uint4 volumeRight;

      //$0090  SND_CTRL
      uint1 enable;
      uint1 voice;
    } r;
  } channel2;

  struct Channel3 {
    struct Registers {
      //$0084-0085  SND_CH3_PITCH
      uint11 pitch;

      //$008a  SND_CH3_VOL
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
  } channel3;

  struct Channel4 {
    struct Registers {
      //$0086-0087  SND_CH4_PITCH
      uint11 pitch;

      //$008b  SND_CH4_VOL
      uint4 volumeLeft;
      uint4 volumeRight;

      //$008e  SND_NOISE
      uint5 noiseMode;

      //$0090  SND_CTRL
      uint1 enable;
      uint1 noise;
    } r;
  } channel4;

  struct Registers {
    //$008f  SND_WAVE_BASE
    uint8 waveBase;

    //$0091  SND_OUTPUT
    uint1 speakerEnable;
    uint2 speakerShift;
    uint1 headphoneEnable;

    //$0092  SND_VOICE_CTRL
    uint2 voiceEnableLeft;
    uint2 voiceEnableRight;
  } r;
};

extern APU apu;
