auto lcdScanline() -> void override;
auto lcdOutput(uint2 color) -> void override;
auto joypWrite(bool p15, bool p14) -> void override;

auto loadRequest(uint id, string name, string type, bool required) -> void override;
auto loadRequest(uint id, string name, bool required) -> void override;
auto saveRequest(uint id, string name) -> void override;

auto videoRefresh(const uint32* data, uint pitch, uint width, uint height) -> void override;
auto audioSample(int16 lsample, int16 rsample) -> void override;
auto inputPoll(uint port, uint device, uint id) -> int16 override;

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
