struct APU : Processor {
  static void Main();
  void main();
  void tick();
  void set_irq_line();
  void set_sample(int16 sample);

  void power();
  void reset();

  uint8 read(uint16 addr);
  void write(uint16 addr, uint8 data);

  void serialize(serializer&);
  APU();

  struct Filter {
    enum : signed { HiPassStrong = 225574, HiPassWeak = 57593, LoPass = 86322413 };

    int64 hipass_strong;
    int64 hipass_weak;
    int64 lopass;

    signed run_hipass_strong(signed sample);
    signed run_hipass_weak(signed sample);
    signed run_lopass(signed sample);
    void serialize(serializer&);
  } filter;

  struct Envelope {
    uint4 speed;
    bool use_speed_as_volume;
    bool loop_mode;

    bool reload_decay;
    uint8 decay_counter;
    uint4 decay_volume;

    unsigned volume() const;
    void clock();
    void serialize(serializer&);
  };

  struct Sweep {
    uint8 shift;
    bool decrement;
    uint3 period;
    uint8 counter;
    bool enable;
    bool reload;
    unsigned rectangle_period;

    bool check_period();
    void clock(unsigned channel);
    void serialize(serializer&);
  };

  struct Rectangle {
    unsigned length_counter;

    Envelope envelope;
    Sweep sweep;

    uint2 duty;
    uint3 duty_counter;

    uint11 period;
    unsigned period_counter;

    void clock_length();
    bool check_period();
    uint8 clock();
    void serialize(serializer&);
  } rectangle[2];

  struct Triangle {
    unsigned length_counter;

    uint8 linear_length;
    bool halt_length_counter;

    uint11 period;
    unsigned period_counter;

    uint5 step_counter;
    uint8 linear_length_counter;
    bool reload_linear;

    void clock_length();
    void clock_linear_length();
    uint8 clock();
    void serialize(serializer&);
  } triangle;

  struct Noise {
    unsigned length_counter;

    Envelope envelope;

    uint4 period;
    unsigned period_counter;

    bool short_mode;
    uint15 lfsr;

    void clock_length();
    uint8 clock();
    void serialize(serializer&);
  } noise;

  struct DMC {
    unsigned length_counter;
    bool irq_pending;

    uint4 period;
    unsigned period_counter;

    bool irq_enable;
    bool loop_mode;

    uint8 dac_latch;
    uint8 addr_latch;
    uint8 length_latch;

    uint15 read_addr;
    unsigned dma_delay_counter;

    uint3 bit_counter;
    bool have_dma_buffer;
    uint8 dma_buffer;

    bool have_sample;
    uint8 sample;

    void start();
    void stop();
    uint8 clock();
    void serialize(serializer&);
  } dmc;

  struct FrameCounter {
    enum : unsigned { NtscPeriod = 14915 };  //~(21.477MHz / 6 / 240hz)

    bool irq_pending;

    uint2 mode;
    uint2 counter;
    signed divider;

    void serialize(serializer&);
  } frame;

  void clock_frame_counter();
  void clock_frame_counter_divider();

  uint8 enabled_channels;
  int16 cartridge_sample;

  int16 rectangle_dac[32];
  int16 dmc_triangle_noise_dac[128][16][16];

  static const uint8 length_counter_table[32];
  static const uint16 ntsc_dmc_period_table[16];
  static const uint16 pal_dmc_period_table[16];
  static const uint16 ntsc_noise_period_table[16];
  static const uint16 pal_noise_period_table[16];
};

extern APU apu;
