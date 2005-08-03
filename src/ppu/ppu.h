class PPUOutput {
public:
bool hires, interlace;
struct {
bool hires, interlace;
}line[239];
uint16 *buffer;
  PPUOutput();
  ~PPUOutput();
};

class PPU {
public:
int _y;
PPUOutput *output;
MMIO      *mmio;
  virtual uint8 vram_read  (uint16 addr) = 0;
  virtual void  vram_write (uint16 addr, uint8 value) = 0;
  virtual uint8 oam_read   (uint16 addr) = 0;
  virtual void  oam_write  (uint16 addr, uint8 value) = 0;
  virtual uint8 cgram_read (uint16 addr) = 0;
  virtual void  cgram_write(uint16 addr, uint8 value) = 0;

  virtual void  latch_counters() = 0;

  virtual void  run() = 0;
  virtual void  scanline() = 0;
  virtual void  frame() = 0;
  virtual void  power() = 0;
  virtual void  reset() = 0;
  virtual void  set_frameskip(int fs);

  PPU();
  ~PPU();
};
