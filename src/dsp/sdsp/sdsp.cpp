/*
  S-DSP emulator
  license: LGPLv2

  Note: this is basically a C++ cothreaded implementation of Shay Green's (blargg's) S-DSP emulator.
  The actual algorithms, timing information, tables, variable names, etc were all from him.
*/

#include "../../base.h"
#define SDSP_CPP

#define REG(n) state.regs[r_##n]
#define VREG(n) state.regs[v.vidx + v_##n]

#include "gaussian.cpp"
#include "counter.cpp"
#include "envelope.cpp"
#include "brr.cpp"
#include "misc.cpp"
#include "voice.cpp"
#include "echo.cpp"

/* timing */

void sDSP::enter() {
  #define tick() scheduler.addclocks_dsp(3 * 8)

  tick(); //temporary to sync with bDSP timing

  loop:

  // 0
  voice_5(voice[0]);
  voice_2(voice[1]);
  tick();

  // 1
  voice_6(voice[0]);
  voice_3(voice[1]);
  tick();

  // 2
  voice_7(voice[0]);
  voice_4(voice[1]);
  voice_1(voice[3]);
  tick();

  // 3
  voice_8(voice[0]);
  voice_5(voice[1]);
  voice_2(voice[2]);
  tick();

  // 4
  voice_9(voice[0]);
  voice_6(voice[1]);
  voice_3(voice[2]);
  tick();

  // 5
  voice_7(voice[1]);
  voice_4(voice[2]);
  voice_1(voice[4]);
  tick();

  // 6
  voice_8(voice[1]);
  voice_5(voice[2]);
  voice_2(voice[3]);
  tick();

  // 7
  voice_9(voice[1]);
  voice_6(voice[2]);
  voice_3(voice[3]);
  tick();

  // 8
  voice_7(voice[2]);
  voice_4(voice[3]);
  voice_1(voice[5]);
  tick();

  // 9
  voice_8(voice[2]);
  voice_5(voice[3]);
  voice_2(voice[4]);
  tick();

  //10
  voice_9(voice[2]);
  voice_6(voice[3]);
  voice_3(voice[4]);
  tick();

  //11
  voice_7(voice[3]);
  voice_4(voice[4]);
  voice_1(voice[6]);
  tick();

  //12
  voice_8(voice[3]);
  voice_5(voice[4]);
  voice_2(voice[5]);
  tick();

  //13
  voice_9(voice[3]);
  voice_6(voice[4]);
  voice_3(voice[5]);
  tick();

  //14
  voice_7(voice[4]);
  voice_4(voice[5]);
  voice_1(voice[7]);
  tick();

  //15
  voice_8(voice[4]);
  voice_5(voice[5]);
  voice_2(voice[6]);
  tick();

  //16
  voice_9(voice[4]);
  voice_6(voice[5]);
  voice_3(voice[6]);
  tick();

  //17
  voice_1(voice[0]);
  voice_7(voice[5]);
  voice_4(voice[6]);
  tick();

  //18
  voice_8(voice[5]);
  voice_5(voice[6]);
  voice_2(voice[7]);
  tick();

  //19
  voice_9(voice[5]);
  voice_6(voice[6]);
  voice_3(voice[7]);
  tick();

  //20
  voice_1(voice[1]);
  voice_7(voice[6]);
  voice_4(voice[7]);
  tick();

  //21
  voice_8(voice[6]);
  voice_5(voice[7]);
  voice_2(voice[0]);
  tick();

  //22
  voice_3a(voice[0]);
  voice_9(voice[6]);
  voice_6(voice[7]);
  echo_22();
  tick();

  //23
  voice_7(voice[7]);
  echo_23();
  tick();

  //24
  voice_8(voice[7]);
  echo_24();
  tick();

  //25
  voice_3b(voice[0]);
  voice_9(voice[7]);
  echo_25();
  tick();

  //26
  echo_26();
  tick();

  //27
  misc_27();
  echo_27();
  tick();

  //28
  misc_28();
  echo_28();
  tick();

  //29
  misc_29();
  echo_29();
  tick();

  //30
  misc_30();
  voice_3c(voice[0]);
  echo_30();
  tick();

  //31
  voice_4(voice[0]);
  voice_1(voice[2]);
  tick();

  goto loop;

  #undef tick
}

/* register interface for S-SMP $00f2,$00f3 */

uint8 sDSP::read(uint8 addr) {
  return state.regs[addr];
}

void sDSP::write(uint8 addr, uint8 data) {
  state.regs[addr] = data;

  if((addr & 0x0f) == v_envx) {
    state.envx_buf = data;
  } else if((addr & 0x0f) == v_outx) {
    state.outx_buf = data;
  } else if(addr == r_kon) {
    state.new_kon = data;
  } else if(addr == r_endx) {
    //always cleared, regardless of data written
    state.endx_buf = 0;
    state.regs[r_endx] = 0;
  }
}

/* initialization */

void sDSP::power() {
  ram = (uint8*)smp.get_spcram_handle(); //TODO: move to sMemory
  memset(&state, 0, sizeof(state_t));

  for(unsigned i = 0; i < 8; i++) {
    memset(&voice[i], 0, sizeof(voice_t));
    voice[i].vbit = 1 << i;
    voice[i].vidx = i * 0x10;
    voice[i].brr_offset = 1;
  }

  reset();
}

void sDSP::reset() {
  REG(flg) = 0xe0;

  state.noise              = 0x4000;
  state.echo_hist_pos      = state.echo_hist;
  state.every_other_sample = 1;
  state.echo_offset        = 0;
  state.counter            = 0;
}

sDSP::sDSP() {
}

sDSP::~sDSP() {
}
