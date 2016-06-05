#if defined(SFC_SUPERGAMEBOY)

struct ICD2 : Emulator::Interface::Bind, GameBoy::Interface::Hook, Cothread {
  shared_pointer<Emulator::Stream> stream;

  static auto Enter() -> void;
  auto main() -> void;

  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset(bool soft = false) -> void;

  //interface.cpp
  auto lcdScanline() -> void override;
  auto lcdOutput(uint2 color) -> void override;
  auto joypWrite(bool p15, bool p14) -> void override;

  auto loadRequest(uint id, string name, string type, bool required) -> void override;
  auto loadRequest(uint id, string name, bool required) -> void override;
  auto saveRequest(uint id, string name) -> void override;

  auto videoRefresh(const uint32* data, uint pitch, uint width, uint height) -> void override;
  auto audioSample(const double* samples, uint channels) -> void override;
  auto inputPoll(uint port, uint device, uint id) -> int16 override;

  //mmio.cpp
  auto read(uint24 addr, uint8 data) -> uint8;
  auto write(uint24 addr, uint8 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  uint revision;

private:
  Emulator::Interface::Bind* bind = nullptr;
  GameBoy::Interface::Hook* hook = nullptr;

  struct Packet {
    auto operator[](uint addr) -> uint8& { return data[addr & 15]; }
    uint8 data[16];
  };
  Packet packet[64];
  uint packetsize;

  uint joyp_id;
  bool joyp15lock;
  bool joyp14lock;
  bool pulselock;
  bool strobelock;
  bool packetlock;
  Packet joyp_packet;
  uint8 packetoffset;
  uint8 bitdata, bitoffset;

  uint8 r6003;      //control port
  uint8 r6004;      //joypad 1
  uint8 r6005;      //joypad 2
  uint8 r6006;      //joypad 3
  uint8 r6007;      //joypad 4
  uint8 r7000[16];  //JOYP packet data
  uint8 mlt_req;    //number of active joypads

  uint8 output[4 * 512];
  uint read_bank;
  uint read_addr;
  uint write_bank;
  uint write_addr;
};

#else

struct ICD2 : Coprocessor {
  auto init() -> void {}
  auto load() -> void {}
  auto unload() -> void {}
  auto power() -> void {}
  auto reset() -> void {}

  auto read(uint24, uint8) -> uint8 { return 0; }
  auto write(uint24, uint8) -> void { return; }

  auto serialize(serializer&) -> void {}

  uint revision;
};

#endif

extern ICD2 icd2;
