struct Palette {
  alwaysinline unsigned operator()(unsigned r, unsigned g, unsigned b) const;
  void update();

private:
  uint32_t color[1024];
  uint32_t red[1024], green[1024], blue[1024];
};

extern Palette palette;
