class Filter {
public:
  virtual void render(
    uint32_t *output, unsigned outpitch, unsigned &outwidth, unsigned &outheight,
    uint16_t *input, unsigned pitch, unsigned *line, unsigned width, unsigned height
  ) = 0;
};

class FilterInterface : public Filter {
public:
  enum FilterType {
    Direct,
    Scanline,
    Scale2x,
    HQ2x,
    NTSC,
  };

  void set(FilterType type);

  void render(
    uint32_t *output, unsigned outpitch, unsigned &outwidth, unsigned &outheight,
    uint16_t *input, unsigned pitch, unsigned *line, unsigned width, unsigned height
  );

  FilterInterface();

private:
  FilterType active_filter;
};

extern FilterInterface filter;
