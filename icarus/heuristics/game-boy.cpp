struct GameBoyCartridge {
  GameBoyCartridge(uint8_t* data, uint size);

  string markup;

  bool black = false;  //cartridge works in DMG+CGB mode
  bool clear = false;  //cartridge works in CGB mode only

  string mapper = "MBC0";
  bool flash = false;
  bool battery = false;
  bool ram = false;
  bool rtc = false;
  bool accelerometer = false;
  bool rumble = false;

  uint flashSize = 0;
  uint romSize = 0;
  uint ramSize = 0;
  uint rtcSize = 0;
};

GameBoyCartridge::GameBoyCartridge(uint8_t* data, uint size) {
  if(size < 0x4000) return;

  uint index = size < 0x8000 ? size : size - 0x8000;
  if(data[index + 0x0104] == 0xce && data[index + 0x0105] == 0xed
  && data[index + 0x0106] == 0x66 && data[index + 0x0107] == 0x66
  && data[index + 0x0108] == 0xcc && data[index + 0x0109] == 0x0d
  && data[index + 0x0147] >= 0x0b && data[index + 0x0147] <= 0x0d
  ) {
    //MMM01 stores header at bottom of data[]
  } else {
    //all other mappers store header at top of data[]
    index = 0;
  }

  black = (data[index + 0x0143] & 0xc0) == 0x80;
  clear = (data[index + 0x0143] & 0xc0) == 0xc0;

  switch(data[index + 0x0147]) {

  case 0x00:
    mapper = "MBC0";
    break;

  case 0x01:
    mapper = "MBC1";
    break;

  case 0x02:
    mapper = "MBC1";
    ram = true;
    break;

  case 0x03:
    mapper = "MBC1";
    battery = true;
    ram = true;
    break;

  case 0x05:
    mapper = "MBC2";
    ram = true;
    break;

  case 0x06:
    mapper = "MBC2";
    battery = true;
    ram = true;
    break;

  case 0x08:
    mapper = "MBC0";
    ram = true;
    break;

  case 0x09:
    mapper = "MBC0";
    battery = true;
    ram = true;
    break;

  case 0x0b:
    mapper = "MMM01";
    break;

  case 0x0c:
    mapper = "MMM01";
    ram = true;
    break;

  case 0x0d:
    mapper = "MMM01";
    battery = true;
    ram = true;
    break;

  case 0x0f:
    mapper = "MBC3";
    battery = true;
    rtc = true;
    break;

  case 0x10:
    mapper = "MBC3";
    battery = true;
    ram = true;
    rtc = true;
    break;

  case 0x11:
    mapper = "MBC3";
    break;

  case 0x12:
    mapper = "MBC3";
    ram = true;
    break;

  case 0x13:
    mapper = "MBC3";
    battery = true;
    ram = true;
    break;

  case 0x19:
    mapper = "MBC5";
    break;

  case 0x1a:
    mapper = "MBC5";
    ram = true;
    break;

  case 0x1b:
    mapper = "MBC5";
    battery = true;
    ram = true;
    break;

  case 0x1c:
    mapper = "MBC5";
    rumble = true;
    break;

  case 0x1d:
    mapper = "MBC5";
    ram = true;
    rumble = true;
    break;

  case 0x1e:
    mapper = "MBC5";
    battery = true;
    ram = true;
    rumble = true;
    break;

  case 0x20:
    mapper = "MBC6";
    flash = true;
    battery = true;
    ram = true;
    break;

  case 0x22:
    mapper = "MBC7";
    battery = true;
    ram = true;
    accelerometer = true;
    rumble = true;
    break;

  case 0xfc:
    mapper = "CAMERA";
    break;

  case 0xfd:
    mapper = "TAMA";
    battery = true;
    ram = true;
    rtc = true;
    break;

  case 0xfe:
    mapper = "HuC3";
    break;

  case 0xff:
    mapper = "HuC1";
    battery = true;
    ram = true;
    break;

  }

  switch(data[index + 0x0148]) { default:
  case 0x00: romSize =   2 * 16 * 1024; break;
  case 0x01: romSize =   4 * 16 * 1024; break;
  case 0x02: romSize =   8 * 16 * 1024; break;
  case 0x03: romSize =  16 * 16 * 1024; break;
  case 0x04: romSize =  32 * 16 * 1024; break;
  case 0x05: romSize =  64 * 16 * 1024; break;
  case 0x06: romSize = 128 * 16 * 1024; break;
  case 0x07: romSize = 256 * 16 * 1024; break;
  case 0x52: romSize =  72 * 16 * 1024; break;
  case 0x53: romSize =  80 * 16 * 1024; break;
  case 0x54: romSize =  96 * 16 * 1024; break;
  }

  if(mapper == "MBC6" && flash) flashSize = 1024 * 1024;

  switch(data[index + 0x0149]) { default:
  case 0x00: ramSize =  0 * 1024; break;
  case 0x01: ramSize =  2 * 1024; break;
  case 0x02: ramSize =  8 * 1024; break;
  case 0x03: ramSize = 32 * 1024; break;
  }

  if(mapper == "MBC2" && ram) ramSize = 256;
  if(mapper == "MBC6" && ram) ramSize =  32 * 1024;
  if(mapper == "MBC7" && ram) ramSize = 256;
  if(mapper == "TAMA" && ram) ramSize =  32;

  if(mapper == "MBC3" && rtc) rtcSize = 13;
  if(mapper == "TAMA" && rtc) rtcSize = 21;

  markup.append("board mapper=", mapper, accelerometer ? " accelerometer" : "", rumble ? " rumble" : "", "\n");
  markup.append("  rom name=program.rom size=0x", hex(romSize), "\n");
  if(flash && flashSize) markup.append("  flash name=download.rom size=0x", hex(flashSize), "\n");
  if(ram && ramSize) markup.append("  ram ", battery ? "name=save.ram " : "", "size=0x", hex(ramSize), "\n");
  if(rtc && rtcSize) markup.append("  rtc ", battery ? "name=rtc.ram " : "", "size=0x", hex(rtcSize), "\n");
}
