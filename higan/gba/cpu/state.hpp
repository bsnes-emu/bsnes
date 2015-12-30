struct Pending {
  struct DMA {
    bool vblank;
    bool hblank;
    bool hdma;
  } dma;
} pending;

struct Active {
  bool dma;
} active;
