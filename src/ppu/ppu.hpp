#include "counter.hpp"

class PPU : public PPUcounter, public MMIO {
public:
  virtual void enter() = 0;

  uint16 *output;

  struct {
    bool render_output;

    bool frame_executed;
    bool frames_updated;
    unsigned frames_rendered;
    unsigned frames_executed;
  } status;

  //PPU1 version number
  //* 1 is known
  //* reported by $213e
  uint8 ppu1_version;

  //PPU2 version number
  //* 1 and 3 are known
  //* reported by $213f
  uint8 ppu2_version;

  virtual bool interlace() const = 0;
  virtual bool overscan() const = 0;
  virtual bool hires() const = 0;

  virtual void latch_counters() = 0;

  virtual void frame();
  virtual void power();
  virtual void reset();
  virtual void enable_renderer(bool r);
  virtual bool renderer_enabled();

  PPU();
  virtual ~PPU();
};
