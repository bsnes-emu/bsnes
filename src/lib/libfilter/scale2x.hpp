class Scale2xFilter : public Filter {
public:
  void render(uint32_t*, unsigned, unsigned&, unsigned&, uint16_t*, unsigned, unsigned*, unsigned, unsigned);
};

extern Scale2xFilter filter_scale2x;
