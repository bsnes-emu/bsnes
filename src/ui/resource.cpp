namespace resource {

#include "../data/icon48.h"
#include "../data/controller.h"

static uint8_t *icon48;
static uint8_t *controller;

//call once at program startup
void init() {
  uint8_t *lzssdata;
  uint8_t *rawdata;
  unsigned length;

  base64::decode(lzssdata, length, enc_icon48);
  lzss::decode(icon48, lzssdata, 48 * 48 * 4);
  delete[] lzssdata;

  //controller data stored as 24-bit RGB888
  //expand to 32-bit ARGB8888 for direct use with hiro::Canvas
  base64::decode(lzssdata, length, enc_controller);
  lzss::decode(rawdata, lzssdata, 372 * 178 * 3);
  delete[] lzssdata;
  controller = new uint8_t[372 * 178 * 4];
  for(unsigned dp = 0, sp = 0, y = 0; y < 178; y++) {
    for(unsigned x = 0; x < 372; x++) {
      controller[dp++] = rawdata[sp++]; //blue
      controller[dp++] = rawdata[sp++]; //green
      controller[dp++] = rawdata[sp++]; //red
      controller[dp++] = 255; //alpha
    }
  }
  delete[] rawdata;
}

} //namespace resource
