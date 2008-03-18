class HQ2xFilter : public Filter {
public:
  void render(uint32_t*, unsigned, unsigned&, unsigned&, uint16_t*, unsigned, unsigned*, unsigned, unsigned);

  HQ2xFilter();
  ~HQ2xFilter();

private:
  uint32_t *yuvtable;
  uint32_t diff_offset;
  uint32_t diff_mask;
};

extern HQ2xFilter filter_hq2x;
