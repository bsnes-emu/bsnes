auto lcdScanline() -> void;
auto lcdOutput(uint2 color) -> void;
auto joypWrite(bool p15, bool p14) -> void;

auto videoColor(uint source, uint16 red, uint16 green, uint16 blue) -> uint32;
auto videoRefresh(const uint32* data, uint pitch, uint width, uint height) -> void;
auto audioSample(int16 lsample, int16 rsample) -> void;
auto inputPoll(uint port, uint device, uint id) -> int16;

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
