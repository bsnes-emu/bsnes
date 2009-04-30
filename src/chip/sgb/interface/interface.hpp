namespace SNES {

static const char command_name[32][64] = {
  "PAL01",    "PAL23",    "PAL03",    "PAL12",
  "ATTR_BLK", "ATTR_LIN", "ATTR_DIV", "ATTR_CHR",
  "SOUND",    "SOU_TRN",  "PAL_SET",  "PAL_TRN",
  "ATRC_EN",  "TEST_EN",  "ICON_EN",  "DATA_SND",
  "DATA_TRN", "MLT_REG",  "JUMP",     "CHR_TRN",
  "PCT_TRN",  "ATTR_TRN", "ATTR_SET", "MASK_EN",
  "OBJ_TRN",  "19_???",   "1A_???",   "1B_???",
  "1C_???",   "1D_???",   "1E_ROM",   "1F_???",
};

class Gameboy {
public:
  uint8_t vram[(160 / 8) * (144 / 8) * (64 / 4)];

  bool pulselock;
  bool strobelock;
  bool packetlock;

  SuperGameboy::Packet packet;
  uint8_t packetoffset;
  uint8_t bitdata, bitoffset;

  unsigned joypadid() {
    return sgb.mmio.joypadid;
  }

  void write(bool p15, bool p14) {
    if(p15 == 1 && p14 == 1) {
      sgb.mmio.joypadid++;
      if(sgb.mmio.r6003 & 0x20) sgb.mmio.joypadid &= 3;
      else if(sgb.mmio.r6003 & 0x10) sgb.mmio.joypadid &= 1;
      else sgb.mmio.joypadid &= 0;
    }

    if(p15 == 0 && p14 == 0) {
      //pulse
      pulselock = false;
      packetoffset = 0;
      bitoffset = 0;
      strobelock = true;
      packetlock = false;
      return;
    }

    if(pulselock) return;

    if(p15 == 1 && p14 == 1) {
      strobelock = false;
      return;
    }

    if(strobelock) return;

    //p15(1), p14(0) = 0
    //p15(0), p14(1) = 1
    bool bit = (p15 == 0);
    strobelock = true;

    if(packetlock) {
      if(p15 == 1 && p14 == 0) {
        //packet stop bit
        printf("%s\n", command_name[packet[0] >> 3]);
        sgb.packet.add(packet);
        packetlock = false;
        pulselock = true;
      }
      return;
    }

    bitdata = (bit << 7) | (bitdata >> 1);
    if(++bitoffset < 8) return;

    bitoffset = 0;
    packet[packetoffset] = bitdata;
    if(++packetoffset < 16) return;
    packetlock = true;
  }

  virtual unsigned run() { return 64; }
  virtual unsigned lyCounter() { return 0; }
  virtual void updateVideo(unsigned) {}
  virtual void unload() {}

  virtual void power() {
    pulselock = true;
    memset(&vram, 0, sizeof vram);
  }

  virtual void reset() {
    pulselock = true;
    memset(&vram, 0, sizeof vram);
  }
};

};
