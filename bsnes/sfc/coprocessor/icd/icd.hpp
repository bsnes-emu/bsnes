struct ICD : Emulator::Platform, Thread {
  shared_pointer<Emulator::Stream> stream;

  inline auto pathID() const -> uint { return information.pathID; }

  static auto Enter() -> void;
  auto main() -> void;

  auto load() -> bool;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;  //software reset

  //interface.cpp
  auto ppuScanline() -> void;
  auto ppuOutput(uint2 color) -> void;
  auto apuOutput(float left, float right) -> void;
  auto joypWrite(bool p14, bool p15) -> void;

  //io.cpp
  auto readIO(uint addr, uint8 data) -> uint8;
  auto writeIO(uint addr, uint8 data) -> void;

  //boot-roms.cpp
  static const uint8_t SGB1BootROM[256];
  static const uint8_t SGB2BootROM[256];

  //serialization.cpp
  auto serialize(serializer&) -> void;

  uint Revision = 0;
  uint Frequency = 0;

private:
  struct Packet {
    auto operator[](uint addr) -> uint8& { return data[addr & 15]; }
    uint8 data[16];
  };
  Packet packet[64];
  uint packetSize;

  uint joypID;
  bool joyp15Lock;
  bool joyp14Lock;
  bool pulseLock;
  bool strobeLock;
  bool packetLock;
  Packet joypPacket;
  uint8 packetOffset;
  uint8 bitData, bitOffset;

  uint8 r6003;      //control port
  uint8 r6004;      //joypad 1
  uint8 r6005;      //joypad 2
  uint8 r6006;      //joypad 3
  uint8 r6007;      //joypad 4
  uint8 r7000[16];  //JOYP packet data
  uint8 mltReq;     //number of active joypads

  uint8 output[4 * 512];
  uint readBank;
  uint readAddress;
  uint writeBank;
  uint writeX;
  uint writeY;

  struct Information {
    uint pathID = 0;
  } information;

public:
  //warning: the size of this object will be too large due to C++ size rules differing from C rules.
  //in practice, this won't pose a problem so long as the struct is never accessed from C++ code,
  //as the offsets of all member variables will be wrong compared to what the C SameBoy code expects.
  GB_gameboy_t sameboy;
  uint32_t bitmap[160 * 144];
  uint8_t ly = 0;
};

extern ICD icd;
