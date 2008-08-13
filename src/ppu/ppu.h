class PPU : public MMIO {
public:
  uint16 *output;

  //this struct should be read-only to
  //functions outside of this class
  struct {
    bool   render_output;

    bool   frame_executed;
    bool   frames_updated;
    uint32 frames_rendered;
    uint32 frames_executed;
  } status;

  //PPU1 version number
  //* 1 is known
  //* reported by $213e
  uint8 ppu1_version;

  //PPU2 version number
  //* 1 and 3 are known
  //* reported by $213f
  uint8 ppu2_version;

  virtual bool field() = 0;
  virtual bool interlace() = 0;
  virtual bool overscan() = 0;
  virtual bool hires() = 0;

  virtual void latch_counters() = 0;

  virtual void render_scanline() = 0;
  virtual void scanline() = 0;
  virtual void frame();
  virtual void power();
  virtual void reset();
  virtual void enable_renderer(bool r);
  virtual bool renderer_enabled();

  PPU();
  virtual ~PPU();
};
