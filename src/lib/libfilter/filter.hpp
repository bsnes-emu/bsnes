class Filter {
public:
  virtual void size(unsigned &outwidth, unsigned &outheight, unsigned width, unsigned height) = 0;

  virtual void render(
    uint32_t *output, unsigned outpitch, uint16_t *input, unsigned pitch,
    unsigned *line, unsigned width, unsigned height
  ) = 0;
};

class FilterInterface : public Filter {
public:
  enum FilterType {
    Direct,
    Scanline,
    Scale2x,
    LQ2x,
    HQ2x,
    NTSC,
  };

  void set(FilterType type);

  void size(unsigned &outwidth, unsigned &outheight, unsigned width, unsigned height);

  void render(
    uint32_t *output, unsigned outpitch, uint16_t *input, unsigned pitch,
    unsigned *line, unsigned width, unsigned height
  );

  FilterInterface();

private:
  FilterType active_filter;
};

extern FilterInterface filter;
