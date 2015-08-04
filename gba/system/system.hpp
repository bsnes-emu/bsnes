enum class Input : unsigned {
  A, B, Select, Start, Right, Left, Up, Down, R, L,
};

struct BIOS : Memory {
  BIOS();
  ~BIOS();

  auto read(unsigned mode, uint32 addr) -> uint32 override;
  auto write(unsigned mode, uint32 addr, uint32 word) -> void override;

  uint8* data = nullptr;
  unsigned size = 0;
  uint32 mdr = 0;
};

struct System {
  void init();
  void term();
  void load();
  void power();
  void run();
  void runtosave();
  void runthreadtosave();

  unsigned serialize_size;

  serializer serialize();
  bool unserialize(serializer&);

  void serialize(serializer&);
  void serialize_all(serializer&);
  void serialize_init();

  struct Information {
    string manifest;
  } information;
};

extern BIOS bios;
extern System system;
