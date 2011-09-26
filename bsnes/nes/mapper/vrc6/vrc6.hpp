struct VRC6 : Mapper {
  void main();

  uint8 prg_read(unsigned addr);
  void prg_write(unsigned addr, uint8 data);

  uint8 chr_read(unsigned addr);
  void chr_write(unsigned addr, uint8 data);

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

    void clock();
    void serialize(serializer&);
  } pulse1, pulse2;

  struct Sawtooth {
    uint6 rate;
    bool enable;
    uint12 frequency;

    uint12 divider;
    uint1 phase;
    uint3 stage;
    uint8 accumulator;
    uint5 output;

    void clock();
    void serialize(serializer&);
  } sawtooth;

  unsigned ciram_addr(unsigned addr) const;
};

extern VRC6 vrc6;
