class PPUcounter {
public:
  alwaysinline void tick() {
    status.hcounter += 2;
    if(status.hcounter >= 1360 && status.hcounter == lineclocks()) {
      status.hcounter = 0;
      status.vcounter++;
      if((region() == 0 && interlace() == false && status.vcounter == 262)
      || (region() == 0 && interlace() == true  && status.vcounter == 263)
      || (region() == 0 && interlace() == true  && status.vcounter == 262 && status.field == 1)
      || (region() == 1 && interlace() == false && status.vcounter == 312)
      || (region() == 1 && interlace() == true  && status.vcounter == 313)
      || (region() == 1 && interlace() == true  && status.vcounter == 312 && status.field == 1)
      ) {
        status.vcounter = 0;
        status.field = !status.field;
      }
      scanline();
    }

    history.index = (history.index + 1) & 2047;
    history.field   [history.index] = status.field;
    history.vcounter[history.index] = status.vcounter;
    history.hcounter[history.index] = status.hcounter;
    history.ppudiff += 2;
  }

  alwaysinline void tock(unsigned clocks) {
    history.ppudiff -= clocks;
  }

  //timing information relative to S-CPU
  alwaysinline bool   field   () const { return status.field; }
  alwaysinline uint16 vcounter() const { return status.vcounter; }
  alwaysinline uint16 hcounter() const { return status.hcounter; }
  uint16 hdot() const;
  uint16 lineclocks() const;

  //timing history information relative to S-CPU
  alwaysinline bool   field   (unsigned offset) const { return history.field   [(history.index - (offset >> 1)) & 2047]; }
  alwaysinline uint16 vcounter(unsigned offset) const { return history.vcounter[(history.index - (offset >> 1)) & 2047]; }
  alwaysinline uint16 hcounter(unsigned offset) const { return history.hcounter[(history.index - (offset >> 1)) & 2047]; }

  //timing information relative to S-PPU
  alwaysinline bool   ifield()    const { return history.field   [(history.index - (history.ppudiff >> 1)) & 2047]; }
  alwaysinline uint16 ivcounter() const { return history.vcounter[(history.index - (history.ppudiff >> 1)) & 2047]; }
  alwaysinline uint16 ihcounter() const { return history.hcounter[(history.index - (history.ppudiff >> 1)) & 2047]; }
  uint16 ilineclocks() const;

  void reset();

private:
  bool region() const;
  bool interlace() const;
  void scanline();

  struct {
    bool field;
    uint16 vcounter;
    uint16 hcounter;
  } status;

  struct {
    bool field[2048];
    uint16 vcounter[2048];
    uint16 hcounter[2048];

    int32 index;
    int32 ppudiff;
  } history;
};
