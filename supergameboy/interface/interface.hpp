class SuperGameBoy : public Gambatte::VideoBlitter, public Gambatte::InputStateGetter {
public:
  Gambatte::GB *gambatte;

//SuperGameBoy::MMIO
  unsigned vram_row;
  uint8_t vram[320];

  struct MMIO {
    uint8_t r6000;
    uint8_t r6003;
    uint8_t r6004;
    uint8_t r6005;
    uint8_t r6006;
    uint8_t r6007;
    uint8_t r7000[16];
    unsigned r7800;
    uint8_t mlt_req;
  } mmio;

//SuperGameBoy::Packet
  static const char command_name[32][64];

  struct Packet {
    uint8_t data[16];
    uint8_t& operator[](unsigned addr) { return data[addr & 15]; }
  };
  Packet packet[64];
  unsigned packetsize;

  unsigned joyp_id;
  bool joyp15lock;
  bool joyp14lock;
  bool pulselock;
  bool strobelock;
  bool packetlock;
  Packet joyp_packet;
  uint8_t packetoffset;
  uint8_t bitdata, bitoffset;

  void joyp_write(bool p15, bool p14);

//SuperGameBoy::Core
  uint8_t *romdata, *ramdata, *rtcdata;
  unsigned romsize,  ramsize,  rtcsize;
  bool version;

  bool init(bool version);
  void term();
  unsigned run(uint32_t *samplebuffer, unsigned samples);
  void save();
  void serialize(nall::serializer &s);
  void power();
  void reset();
  void row(unsigned row);
  uint8_t read(uint16_t addr);
  void write(uint16_t addr, uint8_t data);

  void mmio_reset();
  void command_1e();
  void render(unsigned row);

  SuperGameBoy();
  ~SuperGameBoy();

//Gambatte::VideoBlitter
  unsigned bufferWidth, bufferHeight;
  uint32_t *buffer;

  void setBufferDimensions(unsigned width, unsigned height);
  const Gambatte::PixelBuffer inBuffer();
  void blit();

//Gambatte::InputStateGetter
  Gambatte::InputState inputState;

  const Gambatte::InputState& operator()();
};

extern SuperGameBoy supergameboy;
