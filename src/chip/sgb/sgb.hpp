class Gameboy;

class SuperGameboy : public Memory {
public:
  Gameboy *gameboy;
  void enter();

  struct Packet {
    uint8_t data[16];
    uint8_t& operator[](unsigned addr) { return data[addr & 15]; }
  };
  vector<Packet> packet;

  struct MMIO {
    unsigned r6000;
    uint8_t  r6003;

    //$6004-$6007
    uint8_t  joypadid;
    uint8_t  joypad[4];

    Packet   r7000;
    unsigned r7800;
  } mmio;

  uint8_t read(unsigned addr);
  void write(unsigned addr, uint8_t data);

  void init();
  void enable();
  void power();
  void reset();
  void unload();

  SuperGameboy();
  ~SuperGameboy();

protected:
  uint64_t multiplier;
  uint64_t counter;
  void command_1e();
};

extern SuperGameboy sgb;
