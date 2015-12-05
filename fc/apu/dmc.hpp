struct DMC {
  auto start() -> void;
  auto stop() -> void;
  auto clock() -> uint8;

  auto power() -> void;
  auto reset() -> void;

  auto serialize(serializer&) -> void;

  uint length_counter;
  bool irq_pending;

  uint4 period;
  uint period_counter;

  bool irq_enable;
  bool loop_mode;

  uint8 dac_latch;
  uint8 addr_latch;
  uint8 length_latch;

  uint15 read_addr;
  uint dma_delay_counter;

  uint3 bit_counter;
  bool have_dma_buffer;
  uint8 dma_buffer;

  bool have_sample;
  uint8 sample;
} dmc;
