struct Registers {
  struct SoundBias {
    uint10 level;
    uint2 amplitude;

    operator uint16() const;
    uint16 operator=(uint16 source);
    SoundBias& operator=(const SoundBias&) = delete;
  } bias;
} regs;
