class Screen {
public:
  void render_black();
  void render();
  Screen(PPU &self);
  ~Screen();

private:
  PPU &self;
  uint16 **light_tables;
  uint16 *light_table;

  struct Regs {
  } regs;

  friend class PPU;
};
