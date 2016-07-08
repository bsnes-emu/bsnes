#if defined(SFC_SUPERGAMEBOY)

struct ICD2 : Emulator::Interface::Bind, GameBoy::Interface::Hook, Cothread {
  shared_pointer<Emulator::Stream> stream;

  static auto Enter() -> void;
  auto main() -> void;

  auto init() -> void;
  auto load() -> bool;
  auto unload() -> void;
  auto power() -> void;
  auto reset(bool soft = false) -> void;

  //interface.cpp
  auto lcdScanline() -> void override;
  auto lcdOutput(uint2 color) -> void override;
  auto joypWrite(bool p15, bool p14) -> void override;

  auto open(uint id, string name, vfs::file::mode mode, bool required) -> vfs::shared::file override;
  auto load(uint id, string name, string type) -> maybe<uint> override;

  auto videoRefresh(const uint32* data, uint pitch, uint width, uint height) -> void override;
  auto audioSample(const double* samples, uint channels) -> void override;
  auto inputPoll(uint port, uint device, uint id) -> int16 override;

  //io.cpp
  auto readIO(uint24 addr, uint8 data) -> uint8;
  auto writeIO(uint24 addr, uint8 data) -> void;

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
  uint writeAddress;
};

#else

struct ICD2 : Coprocessor {
  auto init() -> void {}
  auto load() -> void {}
  auto unload() -> void {}
  auto power() -> void {}
  auto reset() -> void {}

  auto readIO(uint24, uint8) -> uint8 { return 0; }
  auto writeIO(uint24, uint8) -> void { return; }

  auto serialize(serializer&) -> void {}

  uint revision;
};

#endif

extern ICD2 icd2;
