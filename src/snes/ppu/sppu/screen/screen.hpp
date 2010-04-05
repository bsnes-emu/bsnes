class Screen {
public:
  sPPU &self;
  uint16 *output;

  void scanline();
  void run();

  Screen(sPPU &self);

private:
  uint16 light_table[16][32768];
};
