#ifndef SUPERGAMEBOY_HPP
#define SUPERGAMEBOY_HPP

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <nall/file.hpp>
#include <nall/serializer.hpp>
using namespace nall;

#include <libgambatte/include/gambatte.h>
#include <interface/interface.hpp>

extern "C" {
  void sgb_rom(uint8_t *data, unsigned size);
  void sgb_ram(uint8_t *data, unsigned size);
  void sgb_rtc(uint8_t *data, unsigned size);
  bool sgb_init(bool version);
  void sgb_term();
  void sgb_power();
  void sgb_reset();
  void sgb_row(unsigned row);
  uint8_t sgb_read(uint16_t addr);
  void sgb_write(uint16_t addr, uint8_t data);
  unsigned sgb_run(uint32_t *samplebuffer, unsigned clocks);
  void sgb_save();
  void sgb_serialize(nall::serializer &s);
}

#endif
