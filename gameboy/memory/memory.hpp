struct Memory {
  uint8_t *data;
  unsigned size;

  uint8_t& operator[](unsigned addr);
  void allocate(unsigned size);
  void copy(const uint8_t *data, unsigned size);
  void free();
  Memory();
  ~Memory();
};

class Bus {
public:
  Memory cartrom;
  Memory cartram;
  Memory vram;
  Memory wram;
  Memory oam;
  Memory hram;

  uint8 read(uint16 addr);
  void write(uint16 addr, uint8 data);

  Bus();
};

extern Bus bus;
