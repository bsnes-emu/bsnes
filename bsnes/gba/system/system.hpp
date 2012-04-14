enum class Input : unsigned {
  A, B, Select, Start, Right, Left, Up, Down, R, L,
};

struct BIOS : Memory {
  uint8 *data;
  unsigned size;
  uint32 mdr;

  void load(const uint8 *data, unsigned size);
  uint32 read(uint32 addr, uint32 size);
  void write(uint32 addr, uint32 size, uint32 word);

  BIOS();
  ~BIOS();
};

struct System {
  void init();
  void term();
  void power();
  void run();
};

extern BIOS bios;
extern System system;
