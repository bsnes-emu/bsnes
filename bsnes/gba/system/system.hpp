enum class Input : unsigned {
  A, B, Select, Start, Right, Left, Up, Down, R, L,
};

struct System {
  struct BIOS : StaticMemory {
    void load(const uint8_t *data, unsigned size);
    BIOS();
  } bios;

  void init();
  void term();
  void power();
  void run();
};

extern System system;
