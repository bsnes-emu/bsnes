class HQ2xFilter : public Filter {
public:
  void size(unsigned &outwidth, unsigned &outheight, unsigned width, unsigned height);
  void render(uint32_t*, unsigned, uint16_t*, unsigned, unsigned*, unsigned, unsigned);

  HQ2xFilter();
  ~HQ2xFilter();

private:
  static const uint8_t hqTable[256];
  uint32_t *yuvTable;
  uint32_t diff_offset;
  uint32_t diff_mask;
  uint8_t rotate[256];

  bool same(uint16_t, uint16_t);
  bool diff(uint32_t, uint16_t);

  void grow(uint32_t&);
  uint16_t pack(uint32_t);

  uint16_t blend1(uint32_t, uint32_t);
  uint16_t blend2(uint32_t, uint32_t, uint32_t);
  uint16_t blend3(uint32_t, uint32_t, uint32_t);
  uint16_t blend4(uint32_t, uint32_t, uint32_t);
  uint16_t blend5(uint32_t, uint32_t, uint32_t);
  uint16_t blend6(uint32_t, uint32_t, uint32_t);
  uint16_t blend (unsigned, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
};

extern HQ2xFilter filter_hq2x;
