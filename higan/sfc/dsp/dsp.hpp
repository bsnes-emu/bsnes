//Sony CXD1222Q-1

struct DSP : Thread {
  shared_pointer<Emulator::Stream> stream;

  DSP();

  alwaysinline auto step(uint clocks) -> void;
  alwaysinline auto synchronizeSMP() -> void;

  auto mute() const -> bool;
  auto read(uint8 addr) -> uint8;
  auto write(uint8 addr, uint8 data) -> void;

  auto main() -> void;
  auto power() -> void;
  auto reset() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

privileged:
  enum GlobalRegister : uint {
    MVOLL = 0x0c, MVOLR = 0x1c,
    EVOLL = 0x2c, EVOLR = 0x3c,
    KON   = 0x4c, KOFF  = 0x5c,
    FLG   = 0x6c, ENDX  = 0x7c,
    EFB   = 0x0d, PMON  = 0x2d,
    NON   = 0x3d, EON   = 0x4d,
    DIR   = 0x5d, ESA   = 0x6d,
    EDL   = 0x7d, FIR   = 0x0f,  //8 coefficients at 0x0f, 0x1f, ... 0x7f
  };

  enum VoiceRegister : uint {
    VOLL   = 0x00, VOLR   = 0x01,
    PITCHL = 0x02, PITCHH = 0x03,
    SRCN   = 0x04, ADSR0  = 0x05,
    ADSR1  = 0x06, GAIN   = 0x07,
    ENVX   = 0x08, OUTX   = 0x09,
  };

  enum EnvelopeMode : uint {
    EnvelopeRelease,
    EnvelopeAttack,
    EnvelopeDecay,
    EnvelopeSustain,
  };

  enum : uint {
    BrrBlockSize = 9,
    CounterRange = 2048 * 5 * 3,  //30720 (0x7800)
  };

  struct State {
    uint8 regs[128];

    int echoHistory[2][8];  //echo history keeps most recent 8 stereo samples
    uint3 echoHistoryOffset;

    bool everyOtherSample;  //toggles every sample
    int kon;                //KON value when last checked
    int noise;
    int counter;
    int echoOffset;         //offset from ESA in echo buffer
    int echoLength;         //number of bytes that echo_offset will stop at

    //hidden registers also written to when main register is written to
    int konBuffer;
    int endxBuffer;
    int envxBuffer;
    int outxBuffer;

    //temporary state between clocks (prefixed with _)

    //read once per sample
    int _pmon;
    int _non;
    int _eon;
    int _dir;
    int _koff;

    //read a few clocks ahead before used
    int _brrNextAddress;
    int _adsr0;
    int _brrHeader;
    int _brrByte;
    int _srcn;
    int _esa;
    int _echoDisabled;

    //internal state that is recalculated every sample
    int _dirAddress;
    int _pitch;
    int _output;
    int _looped;
    int _echoPointer;

    //left/right sums
    int _mainOut[2];
    int _echoOut[2];
    int _echoIn [2];
  } state;

  struct Voice {
    int buffer[12 * 3];  //12 decoded samples (mirrored for wrapping)
    int bufferOffset;    //place in buffer where next samples will be decoded
    int gaussianOffset;  //relative fractional position in sample (0x1000 = 1.0)
    int brrAddress;      //address of current BRR block
    int brrOffset;       //current decoding offset in BRR block
    int vbit;            //bitmask for voice: 0x01 for voice 0, 0x02 for voice 1, etc
    int vidx;            //voice channel register index: 0x00 for voice 0, 0x10 for voice 1, etc
    int konDelay;        //KON delay/current setup phase
    int envelopeMode;
    int envelope;        //current envelope level
    int hiddenEnvelope;  //used by GAIN mode 7, very obscure quirk
    int _envxOut;
  } voice[8];

  //gaussian.cpp
  static const int16 GaussianTable[512];
  auto gaussianInterpolate(const Voice& v) -> int;

  //counter.cpp
  static const uint16 CounterRate[32];
  static const uint16 CounterOffset[32];
  auto counterTick() -> void;
  auto counterPoll(uint rate) -> bool;

  //envelope.cpp
  auto envelopeRun(Voice& v) -> void;

  //brr.cpp
  auto brrDecode(Voice& v) -> void;

  //misc.cpp
  auto misc27() -> void;
  auto misc28() -> void;
  auto misc29() -> void;
  auto misc30() -> void;

  //voice.cpp
  auto voiceOutput(Voice& v, bool channel) -> void;
  auto voice1 (Voice& v) -> void;
  auto voice2 (Voice& v) -> void;
  auto voice3 (Voice& v) -> void;
  auto voice3a(Voice& v) -> void;
  auto voice3b(Voice& v) -> void;
  auto voice3c(Voice& v) -> void;
  auto voice4 (Voice& v) -> void;
  auto voice5 (Voice& v) -> void;
  auto voice6 (Voice& v) -> void;
  auto voice7 (Voice& v) -> void;
  auto voice8 (Voice& v) -> void;
  auto voice9 (Voice& v) -> void;

  //echo.cpp
  auto calculateFIR(bool channel, int index) -> int;
  auto echoOutput(bool channel) -> int;
  auto echoRead(bool channel) -> void;
  auto echoWrite(bool channel) -> void;
  auto echo22() -> void;
  auto echo23() -> void;
  auto echo24() -> void;
  auto echo25() -> void;
  auto echo26() -> void;
  auto echo27() -> void;
  auto echo28() -> void;
  auto echo29() -> void;
  auto echo30() -> void;

  //dsp.cpp
  static auto Enter() -> void;
  auto tick() -> void;
};

extern DSP dsp;
