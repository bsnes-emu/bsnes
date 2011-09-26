struct VRC6 : Mapper {
  void main();

  uint8 prg_read(uint16 addr);
  void prg_write(uint16 addr, uint8 data);

  uint8 chr_read(uint16 addr);
  void chr_write(uint16 addr, uint8 data);

  uint8 ciram_read(uint13 addr);
  void ciram_write(uint13 addr, uint8 data);

  unsigned ram_size();
  uint8* ram_data();

  void power();
  void reset();

  void serialize(serializer&);

//privileged:
  bool abus_swap;

private:
  uint8 prg_ram[8192];

  uint8 prg_bank[2];
  uint8 chr_bank[8];
  uint2 mirror_select;
  uint8 irq_latch;
  bool irq_mode;
  bool irq_enable;
  bool irq_acknowledge;

  uint8 irq_counter;
  signed irq_scalar;
  bool irq_line;

  struct Pulse {
    bool mode;
    uint3 duty;
    uint4 volume;
    bool enable;
    uint12 frequency;

    uint12 divider;
    uint4 cycle;
    uint4 output;
    int16 dac[16];

    void clock();
    void serialize(serializer&);
    Pulse();
  } pulse1, pulse2;

  struct Sawtooth {
    uint6 rate;
    bool enable;
    uint12 frequency;

    uint12 divider;
    uint4 phase;
    uint8 accumulator;
    uint5 output;
    int16 dac[32];

    void clock();
    void serialize(serializer&);
    Sawtooth();
  } sawtooth;

  unsigned ciram_addr(unsigned addr) const;
};

extern VRC6 vrc6;
