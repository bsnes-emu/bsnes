#include "../../base.h"
#include "spc_dsp.h"

void bDSP::power() {
  spc_dsp_init(r_smp->get_spcram_handle());
  spc_dsp_reset();
}

void bDSP::reset() {
  spc_dsp_soft_reset();
}

uint8 bDSP::read(uint8 addr) {
  return spc_dsp_read(addr);
}

void bDSP::write(uint8 addr, uint8 data) {
  spc_dsp_write(addr, data);
}

#define SPC_DSP_CUSTOM_RUN 1 //causes spc_dsp_run() to not be defined since it's huge and we don't need it
#define SPC_DSP_OUT_HOOK(left, right) snes.audio_update(left, right);
#include "spc_dsp.cpp"

void bDSP::enter() { loop:
#define PHASE(n) scheduler.addclocks_dsp(3);
#include "spc_dsp_timing.h"
  goto loop;
}

bDSP::bDSP() {}
bDSP::~bDSP() {}
