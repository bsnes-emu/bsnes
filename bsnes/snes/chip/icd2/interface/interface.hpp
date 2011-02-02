void joyp_write(bool p15, bool p14);
void video_refresh(const uint8_t *data);
void audio_sample(int16_t center, int16_t left, int16_t right);
void input_poll();
bool input_poll(unsigned id);

struct Packet {
  uint8 data[16];
  uint8& operator[](unsigned addr) { return data[addr & 15]; }
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
uint8 packetoffset;
uint8 bitdata, bitoffset;
