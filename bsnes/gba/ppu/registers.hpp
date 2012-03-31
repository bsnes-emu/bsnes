struct Registers {
  struct Control {
    uint3 bgmode;
    bool cgbmode;
    bool frame;
    bool hblank;
    bool objmap;
    bool forceblank;
    bool enablebg[4];
    bool enableobj;
    bool enablebgwindow0;
    bool enablebgwindow1;
    bool enableobjwindow;

    operator uint16() const;
    uint16 operator=(uint16 source);
    Control& operator=(const Control&) = delete;
  } control;

  bool greenswap;

  struct Status {
    bool vblank;
    bool hblank;
    bool vcoincidence;
    bool irqvblank;
    bool irqhblank;
    bool irqvcoincidence;
    uint8 vcompare;

    operator uint16() const;
    uint16 operator=(uint16 source);
    Status& operator=(const Status&) = delete;
  } status;

  uint16 vcounter;

  struct BackgroundControl {
    uint2 priority;
    uint2 characterbaseblock;
    uint1 mosaic;
    uint1 colormode;
    uint5 screenbaseblock;
    uint2 screensize;

    operator uint16() const;
    uint16 operator=(uint16 source);
    BackgroundControl& operator=(const BackgroundControl&) = delete;
  };

  struct Background {
    BackgroundControl control;
    uint9 hoffset;
    uint9 voffset;

    //BG2,3 only
    uint16 pa, pb, pc, pd;
    uint28 x, y;
  } bg[4];

  struct WindowFlags {
    bool enablebg[4];
    bool enableobj;
    bool enablesfx;

    operator uint8() const;
    uint8 operator=(uint8 source);
    WindowFlags& operator=(const WindowFlags&) = delete;
  };

  struct Window {
    uint8 x1, x2;
    uint8 y1, y2;
    WindowFlags in, out;
  } window[2];

  struct ObjectWindow {
    WindowFlags in;
  } windowobj;

  struct Mosaic {
    uint4 bghsize;
    uint4 bgvsize;
    uint4 objhsize;
    uint4 objvsize;
  } mosaic;

  struct BlendControl {
    bool firstbg[4];
    bool firstobj;
    bool firstbd;
    uint2 effect;
    bool secondbg[4];
    bool secondobj;
    bool secondbd;

    operator uint16() const;
    uint16 operator=(uint16 source);
    BlendControl& operator=(const BlendControl&) = delete;
  };

  struct Blend {
    BlendControl control;
    uint5 eva;
    uint5 evb;
    uint5 evy;
  } blend;
} regs;
