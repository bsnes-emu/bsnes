enum {
  MAP_ROM,
  MAP_RAM,
  MAP_BSX,
  MAP_SDD1,
  MAP_CX4,
  MAP_DSP1,
  MAP_DSP2,
  MAP_OBC1,
};

  void cart_map_range(uint8 bank_lo, uint8 bank_hi, uint8 page_lo, uint8 page_hi, uint type, uint offset = 0);

#define mapper(name) void cart_map_##name()
  mapper(shvc_1a3b_13);
  mapper(shvc_1a3b_20);
  mapper(shvc_1a3m_30);
  mapper(bsc_1a5m_01);
  mapper(bsc_1a7m_01);
  mapper(bsc_1a7m_10);
#undef mapper
