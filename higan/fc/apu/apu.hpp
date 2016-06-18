struct APU : Thread {
  shared_pointer<Emulator::Stream> stream;

  APU();

  static auto Enter() -> void;
  auto main() -> void;
  auto tick() -> void;
  auto set_irq_line() -> void;
  auto set_sample(int16 sample) -> void;

  auto power() -> void;
  auto reset() -> void;

  auto readIO(uint16 addr) -> uint8;
  auto writeIO(uint16 addr, uint8 data) -> void;

  auto serialize(serializer&) -> void;

  struct Filter {
    auto run_hipass_strong(int sample) -> int;
    auto run_hipass_weak(int sample) -> int;
    auto run_lopass(int sample) -> int;

    auto serialize(serializer&) -> void;

    enum : int { HiPassStrong = 225574, HiPassWeak = 57593, LoPass = 86322413 };

    int64 hipass_strong;
    int64 hipass_weak;
    int64 lopass;
  };

  #include "envelope.hpp"
  #include "sweep.hpp"
  #include "pulse.hpp"
  #include "triangle.hpp"
  #include "noise.hpp"
  #include "dmc.hpp"

  struct FrameCounter {
    auto serialize(serializer&) -> void;

    enum : uint { NtscPeriod = 14915 };  //~(21.477MHz / 6 / 240hz)

    bool irq_pending;

    uint2 mode;
    uint2 counter;
    int divider;
  };

  auto clock_frame_counter() -> void;
  auto clock_frame_counter_divider() -> void;

  Filter filter;
  FrameCounter frame;

  uint8 enabled_channels;
  int16 cartridge_sample;

  int16 pulse_dac[32];
  int16 dmc_triangle_noise_dac[128][16][16];

  static const uint8 length_counter_table[32];
  static const uint16 ntsc_dmc_period_table[16];
  static const uint16 pal_dmc_period_table[16];
  static const uint16 ntsc_noise_period_table[16];
  static const uint16 pal_noise_period_table[16];
};

extern APU apu;
