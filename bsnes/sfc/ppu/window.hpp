struct Window {
  auto scanline() -> void;
  auto run() -> void;
  auto test(bool oneEnable, bool one, bool twoEnable, bool two, uint mask) -> bool;
  auto power() -> void;

  auto serialize(serializer&) -> void;

  struct IO {
    struct Layer {
      bool oneEnable;
      bool oneInvert;
      bool twoEnable;
      bool twoInvert;
      uint2 mask;
      bool aboveEnable;
      bool belowEnable;
    } bg1, bg2, bg3, bg4, obj;

    struct Color {
      bool oneEnable;
      bool oneInvert;
      bool twoEnable;
      bool twoInvert;
      uint2 mask;
      uint2 aboveMask;
      uint2 belowMask;
    } col;

    uint8 oneLeft;
    uint8 oneRight;
    uint8 twoLeft;
    uint8 twoRight;
  } io;

  struct Output {
    struct Pixel {
      bool colorEnable;
    } above, below;
  } output;

  struct {
    uint x;

    //each window opens when the dot counter reaches its left edge and stays
    //closed for the rest of the line once it has passed its right edge
    //cleared at the start of every scanline, which is where the PPU thread
    //synchronizes for save states, so not serialized
    bool oneActive;
    bool oneStopped;
    bool twoActive;
    bool twoStopped;
  };

  friend class PPU;
};
