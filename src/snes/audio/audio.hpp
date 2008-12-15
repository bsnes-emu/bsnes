class Audio {
public:
  void update(uint16 l_sample, uint16 r_sample);
  void init();

  friend class SNES;
} audio;
