enum class Input : unsigned {
  A, B, Select, Start, Right, Left, Up, Down, R, L,
};

struct System : property<System> {
  struct BIOS {
    uint8_t *data;
    unsigned size;
    void load(const uint8_t *data, unsigned size);
    BIOS();
    ~BIOS();
  } bios;

  void run();

  void init();
  void term();
  void load();
  void unload();
  void power();
};

extern System system;
