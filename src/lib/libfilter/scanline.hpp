class ScanlineFilter : public Filter {
public:
  void size(unsigned &outwidth, unsigned &outheight, unsigned width, unsigned height);
  void render(uint32_t*, unsigned, uint16_t*, unsigned, unsigned*, unsigned, unsigned);
};

extern ScanlineFilter filter_scanline;
