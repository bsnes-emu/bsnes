#include "supergameboy.hpp"

#ifdef _WIN32
  #define dllexport __declspec(dllexport)
#else
  #define dllexport
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <interface/interface.cpp>

dllexport void sgb_rom(uint8_t *data, unsigned size) {
  supergameboy.romdata = data;
  supergameboy.romsize = size;
}

dllexport void sgb_ram(uint8_t *data, unsigned size) {
  supergameboy.ramdata = data;
  supergameboy.ramsize = size;
}

dllexport void sgb_rtc(uint8_t *data, unsigned size) {
  supergameboy.rtcdata = data;
  supergameboy.rtcsize = size;
}

dllexport bool sgb_init(bool version) {
  return supergameboy.init(version);
}

dllexport void sgb_term() {
  supergameboy.term();
}

dllexport void sgb_power() {
  supergameboy.power();
}

dllexport void sgb_reset() {
  supergameboy.reset();
}

dllexport void sgb_row(unsigned row) {
  supergameboy.row(row);
}

dllexport uint8_t sgb_read(uint16_t addr) {
  return supergameboy.read(addr);
}

dllexport void sgb_write(uint16_t addr, uint8_t data) {
  supergameboy.write(addr, data);
}

dllexport unsigned sgb_run(uint32_t *samplebuffer, unsigned clocks) {
  return supergameboy.run(samplebuffer, clocks);
}

dllexport void sgb_save() {
  supergameboy.save();
}

dllexport void sgb_serialize(nall::serializer &s) {
  supergameboy.serialize(s);
}
