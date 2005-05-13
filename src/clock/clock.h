class Clock {
private:
struct {
  bool   overscan;
  bool   interlace;
  bool   interlace_field;

  uint16 vcounter;
  uint16 hcounter;
  uint16 hcycles;
}status;

public:
  virtual void   enable_overscan(bool n);
  virtual void   enable_interlace(bool n);
  virtual bool   overscan();
  virtual bool   interlace();
  virtual bool   interlace_field();
  virtual uint16 vcounter();
  virtual uint16 hcounter();
  virtual uint16 hcycles();
  virtual uint16 visible_scanlines();

  virtual void set_frameskip(uint8 fs);
  virtual bool update_frame() = 0;

  virtual void add_cc1_cycles(uint32 cycles) = 0;
  virtual void add_cc2_cycles(uint32 cycles) = 0;
  virtual void sync() = 0;
  virtual void run() = 0;
  virtual void power() = 0;
  virtual void reset() = 0;

  Clock();

  friend class bClock;
};
