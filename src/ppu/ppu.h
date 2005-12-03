class PPU {
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

MMIO *mmio;

struct scanline_info {
  bool hires;
  bool interlace;
};
  virtual bool  scanline_is_hires() = 0;
  virtual void  get_scanline_info(scanline_info *info);

  virtual uint8 vram_read  (uint16 addr) = 0;
  virtual void  vram_write (uint16 addr, uint8 value) = 0;
  virtual uint8 oam_read   (uint16 addr) = 0;
  virtual void  oam_write  (uint16 addr, uint8 value) = 0;
  virtual uint8 cgram_read (uint16 addr) = 0;
  virtual void  cgram_write(uint16 addr, uint8 value) = 0;

  virtual void  latch_counters() = 0;

  virtual void  run() = 0;
  virtual void  scanline() = 0;
  virtual void  render_scanline() = 0;
  virtual void  frame();
  virtual void  power();
  virtual void  reset();
  virtual void  enable_renderer(bool r);
  virtual bool  renderer_enabled();

  PPU();
  virtual ~PPU();
};
