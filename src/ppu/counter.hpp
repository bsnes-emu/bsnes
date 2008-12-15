class PPUcounter {
public:
  alwaysinline void tick() {
    history.ppudiff += 2;
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
  }

  alwaysinline void tock(unsigned clocks) {
    history.ppudiff -= clocks;
  }

  alwaysinline bool   field   () { return status.field; }
  alwaysinline uint16 vcounter() { return status.vcounter; }
  alwaysinline uint16 hcounter() { return status.hcounter; }
  uint16 hdot();
  uint16 lineclocks();

  alwaysinline bool   field   (unsigned offset) { return history.field   [(history.index - (offset >> 1)) & 2047]; }
  alwaysinline uint16 vcounter(unsigned offset) { return history.vcounter[(history.index - (offset >> 1)) & 2047]; }
  alwaysinline uint16 hcounter(unsigned offset) { return history.hcounter[(history.index - (offset >> 1)) & 2047]; }

  alwaysinline bool   ppufield()    { return history.field   [(history.index - (history.ppudiff >> 1)) & 2047]; }
  alwaysinline uint16 ppuvcounter() { return history.vcounter[(history.index - (history.ppudiff >> 1)) & 2047]; }
  alwaysinline uint16 ppuhcounter() { return history.hcounter[(history.index - (history.ppudiff >> 1)) & 2047]; }
  uint16 ppulineclocks();

  void reset();

private:
  bool region();
  bool interlace();
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

    unsigned index;
    signed ppudiff;
  } history;
};

extern PPUcounter ppucounter;
