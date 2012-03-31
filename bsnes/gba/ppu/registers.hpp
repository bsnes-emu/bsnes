struct Registers {
  struct Control {
    uint3 bgmode;
    bool cgbmode;
    bool frame;
    bool hblank;
    bool objmap;
    bool forceblank;
    bool displaybg0;
    bool displaybg1;
    bool displaybg2;
    bool displaybg3;
    bool displayobj;
    bool displaybgwindow0;
    bool displaybgwindow1;
    bool displayobjwindow;

    operator uint16() const;
    uint16 operator=(uint16 source);
    Control& operator=(const Control&) = delete;
  } control;

  uint16 scanline;
} regs;
