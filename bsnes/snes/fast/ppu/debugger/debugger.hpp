class PPUDebugger : public PPU, public ChipDebugger {
public:
  bool property(unsigned id, string &name, string &value);

  bool bg1_enabled[2];
  bool bg2_enabled[2];
  bool bg3_enabled[2];
  bool bg4_enabled[2];
  bool oam_enabled[4];

  uint8 vram_mmio_read(uint16 addr);
  void vram_mmio_write(uint16 addr, uint8 data);

  uint8 oam_mmio_read(uint16 addr);
  void oam_mmio_write(uint16 addr, uint8 data);

  uint8 cgram_mmio_read(uint16 addr);
  void cgram_mmio_write(uint16 addr, uint8 data);

  void render_line_mode0();
  void render_line_mode1();
  void render_line_mode2();
  void render_line_mode3();
  void render_line_mode4();
  void render_line_mode5();
  void render_line_mode6();
  void render_line_mode7();

  PPUDebugger();
};
