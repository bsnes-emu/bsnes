class ScanlineFilter : public Filter {
public:
  void render(uint32_t*, unsigned, unsigned&, unsigned&, uint16_t*, unsigned, unsigned*, unsigned, unsigned);
};

extern ScanlineFilter filter_scanline;
