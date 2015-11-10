//Sony CXD1222Q-1

struct DSP : Thread {
  enum : bool { Threaded = true };

  DSP();

  alwaysinline auto step(unsigned clocks) -> void;
  alwaysinline auto synchronizeSMP() -> void;

  auto mute() const -> bool;
  auto read(uint8 addr) -> uint8;
  auto write(uint8 addr, uint8 data) -> void;

  auto enter() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto serialize(serializer&) -> void;

privileged:
  #include "modulo-array.hpp"

  enum GlobalRegister : unsigned {
    MVOLL = 0x0c, MVOLR = 0x1c,
    EVOLL = 0x2c, EVOLR = 0x3c,
    KON   = 0x4c, KOFF  = 0x5c,
    FLG   = 0x6c, ENDX  = 0x7c,
    EFB   = 0x0d, PMON  = 0x2d,
    NON   = 0x3d, EON   = 0x4d,
    DIR   = 0x5d, ESA   = 0x6d,
    EDL   = 0x7d, FIR   = 0x0f,  //8 coefficients at 0x0f, 0x1f, ... 0x7f
  };

  enum VoiceRegister : unsigned {
    VOLL   = 0x00, VOLR   = 0x01,
    PITCHL = 0x02, PITCHH = 0x03,
    SRCN   = 0x04, ADSR0  = 0x05,
    ADSR1  = 0x06, GAIN   = 0x07,
    ENVX   = 0x08, OUTX   = 0x09,
  };

  enum EnvelopeMode : unsigned {
    EnvelopeRelease,
    EnvelopeAttack,
    EnvelopeDecay,
    EnvelopeSustain,
  };

  enum : unsigned {
    EchoHistorySize = 8,
    BrrBufferSize = 12,
    BrrBlockSize = 9,
    CounterRange = 2048 * 5 * 3,  //30720 (0x7800)
  };

  struct State {
    uint8 regs[128];

    ModuloArray<signed, EchoHistorySize> echoHistory[2];  //echo history keeps most recent 8 samples
    signed echoHistoryOffset;

    bool everyOtherSample;  //toggles every sample
    signed kon;             //KON value when last checked
    signed noise;
    signed counter;
    signed echoOffset;      //offset from ESA in echo buffer
    signed echoLength;      //number of bytes that echo_offset will stop at

    //hidden registers also written to when main register is written to
    signed konBuffer;
    signed endxBuffer;
    signed envxBuffer;
    signed outxBuffer;

    //temporary state between clocks (prefixed with _)

    //read once per sample
    signed _pmon;
    signed _non;
    signed _eon;
    signed _dir;
    signed _koff;

    //read a few clocks ahead before used
    signed _brrNextAddress;
    signed _adsr0;
    signed _brrHeader;
    signed _brrByte;
    signed _srcn;
    signed _esa;
    signed _echoDisabled;

    //internal state that is recalculated every sample
    signed _dirAddress;
    signed _pitch;
    signed _output;
    signed _looped;
    signed _echoPointer;

    //left/right sums
    signed _mainOut[2];
    signed _echoOut[2];
    signed _echoIn [2];
  } state;

  struct Voice {
    ModuloArray<signed, BrrBufferSize> buffer;  //decoded samples
    signed bufferOffset;    //place in buffer where next samples will be decoded
    signed gaussianOffset;  //relative fractional position in sample (0x1000 = 1.0)
    signed brrAddress;      //address of current BRR block
    signed brrOffset;       //current decoding offset in BRR block
    signed vbit;            //bitmask for voice: 0x01 for voice 0, 0x02 for voice 1, etc
    signed vidx;            //voice channel register index: 0x00 for voice 0, 0x10 for voice 1, etc
    signed konDelay;        //KON delay/current setup phase
    signed envelopeMode;
    signed envelope;        //current envelope level
    signed hiddenEnvelope;  //used by GAIN mode 7, very obscure quirk
    signed _envxOut;
  } voice[8];

  //gaussian
  static const int16 GaussianTable[512];
  auto gaussianInterpolate(const Voice& v) -> signed;

  //counter
  static const uint16 CounterRate[32];
  static const uint16 CounterOffset[32];
  auto counterTick() -> void;
  auto counterPoll(unsigned rate) -> bool;

  //envelope
  auto envelopeRun(Voice& v) -> void;

  //brr
  auto brrDecode(Voice& v) -> void;

  //misc
  auto misc27() -> void;
  auto misc28() -> void;
  auto misc29() -> void;
  auto misc30() -> void;

  //voice
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

  //echo
  auto calculateFIR(signed i, bool channel) -> signed;
  auto echoOutput(bool channel) -> signed;
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

  //dsp
  static auto Enter() -> void;
  auto tick() -> void;
};

extern DSP dsp;
