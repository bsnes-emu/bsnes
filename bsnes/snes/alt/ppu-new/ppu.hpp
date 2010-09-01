class PPU : public Processor, public PPUcounter, public MMIO {
public:
  enum : bool { Threaded = true };
  alwaysinline void step(unsigned clocks);
  alwaysinline void synchronize_cpu();

  void latch_counters();
  bool interlace() const;
  bool overscan() const;
  bool hires() const;

  void enter();
  void power();
  void reset();
  void scanline();
  void frame();

  void serialize(serializer&);
  PPU();
  ~PPU();

private:
  uint16 *surface;
  uint16 *output;

  #include "mmio/mmio.hpp"
  #include "screen/screen.hpp"

  Screen screen;

  struct Display {
    unsigned width;
    unsigned height;
  } display;

  static void Enter();
  void add_clocks(unsigned clocks);
  void render_scanline();

  friend class PPU::Screen;
  friend class Video;
};

extern PPU ppu;
