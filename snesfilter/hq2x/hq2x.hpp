class HQ2xFilter {
public:
  void size(unsigned&, unsigned&, unsigned, unsigned);
  void render(uint32_t*, unsigned, const uint16_t*, unsigned, const unsigned*, unsigned, unsigned);

  HQ2xFilter();
  ~HQ2xFilter();

private:
  enum {
    diff_offset = (0x440 << 21) + (0x207 << 11) + 0x407,
    diff_mask   = (0x380 << 21) + (0x1f0 << 11) + 0x3f0,
  };

  static const uint8_t hqTable[256];
  uint32_t *yuvTable;
  uint8_t rotate[256];

  alwaysinline bool same(uint16_t x, uint16_t y);
  alwaysinline bool diff(uint32_t x, uint16_t y);
  alwaysinline void grow(uint32_t &n);
  alwaysinline uint16_t pack(uint32_t n);
  alwaysinline uint16_t blend1(uint32_t A, uint32_t B);
  alwaysinline uint16_t blend2(uint32_t A, uint32_t B, uint32_t C);
  alwaysinline uint16_t blend3(uint32_t A, uint32_t B, uint32_t C);
  alwaysinline uint16_t blend4(uint32_t A, uint32_t B, uint32_t C);
  alwaysinline uint16_t blend5(uint32_t A, uint32_t B, uint32_t C);
  alwaysinline uint16_t blend6(uint32_t A, uint32_t B, uint32_t C);
  alwaysinline uint16_t blend(unsigned rule, uint16_t E, uint16_t A, uint16_t B, uint16_t D, uint16_t F, uint16_t H);
} filter_hq2x;
