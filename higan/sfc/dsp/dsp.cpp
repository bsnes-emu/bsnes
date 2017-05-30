#include <sfc/sfc.hpp>

namespace SuperFamicom {

DSP dsp;

#define REG(n) state.regs[n]
#define VREG(n) state.regs[v.vidx + n]

#include "gaussian.cpp"
#include "counter.cpp"
#include "envelope.cpp"
#include "brr.cpp"
#include "misc.cpp"
#include "voice.cpp"
#include "echo.cpp"
#include "serialization.cpp"

DSP::DSP() {
  static_assert(sizeof(int) >= 32 / 8,      "int >= 32-bits");
  static_assert((int8_t)0x80 == -0x80,      "8-bit sign extension");
  static_assert((int16_t)0x8000 == -0x8000, "16-bit sign extension");
  static_assert((uint16_t)0xffff0000 == 0,  "16-bit unsigned clip");
  static_assert((-1 >> 1) == -1,            "arithmetic shift right");

  //-0x8000 <= n <= +0x7fff
  assert(sclamp<16>(+0x8000) == +0x7fff);
  assert(sclamp<16>(-0x8001) == -0x8000);
}

/* timing */

auto DSP::step(uint clocks) -> void {
  Thread::step(clocks);
}

auto DSP::Enter() -> void {
  while(true) scheduler.synchronize(), dsp.main();
}

auto DSP::main() -> void {
  voice5(voice[0]);
  voice2(voice[1]);
  tick();

  voice6(voice[0]);
  voice3(voice[1]);
  tick();

  voice7(voice[0]);
  voice4(voice[1]);
  voice1(voice[3]);
  tick();

  voice8(voice[0]);
  voice5(voice[1]);
  voice2(voice[2]);
  tick();

  voice9(voice[0]);
  voice6(voice[1]);
  voice3(voice[2]);
  tick();

  voice7(voice[1]);
  voice4(voice[2]);
  voice1(voice[4]);
  tick();

  voice8(voice[1]);
  voice5(voice[2]);
  voice2(voice[3]);
  tick();

  voice9(voice[1]);
  voice6(voice[2]);
  voice3(voice[3]);
  tick();

  voice7(voice[2]);
  voice4(voice[3]);
  voice1(voice[5]);
  tick();

  voice8(voice[2]);
  voice5(voice[3]);
  voice2(voice[4]);
  tick();

  voice9(voice[2]);
  voice6(voice[3]);
  voice3(voice[4]);
  tick();

  voice7(voice[3]);
  voice4(voice[4]);
  voice1(voice[6]);
  tick();

  voice8(voice[3]);
  voice5(voice[4]);
  voice2(voice[5]);
  tick();

  voice9(voice[3]);
  voice6(voice[4]);
  voice3(voice[5]);
  tick();

  voice7(voice[4]);
  voice4(voice[5]);
  voice1(voice[7]);
  tick();

  voice8(voice[4]);
  voice5(voice[5]);
  voice2(voice[6]);
  tick();

  voice9(voice[4]);
  voice6(voice[5]);
  voice3(voice[6]);
  tick();

  voice1(voice[0]);
  voice7(voice[5]);
  voice4(voice[6]);
  tick();

  voice8(voice[5]);
  voice5(voice[6]);
  voice2(voice[7]);
  tick();

  voice9(voice[5]);
  voice6(voice[6]);
  voice3(voice[7]);
  tick();

  voice1(voice[1]);
  voice7(voice[6]);
  voice4(voice[7]);
  tick();

  voice8(voice[6]);
  voice5(voice[7]);
  voice2(voice[0]);
  tick();

  voice3a(voice[0]);
  voice9(voice[6]);
  voice6(voice[7]);
  echo22();
  tick();

  voice7(voice[7]);
  echo23();
  tick();

  voice8(voice[7]);
  echo24();
  tick();

  voice3b(voice[0]);
  voice9(voice[7]);
  echo25();
  tick();

  echo26();
  tick();

  misc27();
  echo27();
  tick();

  misc28();
  echo28();
  tick();

  misc29();
  echo29();
  tick();

  misc30();
  voice3c(voice[0]);
  echo30();
  tick();

  voice4(voice[0]);
  voice1(voice[2]);
  tick();
}

auto DSP::tick() -> void {
  step(3 * 8);
  synchronize(smp);
}

/* register interface for S-SMP $00f2,$00f3 */

auto DSP::mute() const -> bool {
  return REG(FLG) & 0x40;
}

auto DSP::read(uint8 addr) -> uint8 {
  return REG(addr);
}

auto DSP::write(uint8 addr, uint8 data) -> void {
  REG(addr) = data;

  if((addr & 0x0f) == ENVX) {
    state.envxBuffer = data;
  } else if((addr & 0x0f) == OUTX) {
    state.outxBuffer = data;
  } else if(addr == KON) {
    state.konBuffer = data;
  } else if(addr == ENDX) {
    //always cleared, regardless of data written
    state.endxBuffer = 0;
    REG(ENDX) = 0;
  }
}

/* initialization */

auto DSP::load(Markup::Node node) -> bool {
  return true;
}

auto DSP::power() -> void {
  create(Enter, 32040.0 * 768.0);
  stream = Emulator::audio.createStream(2, frequency() / 768.0);

  memory::fill(&state, sizeof(State));
  state.noise = 0x4000;
  state.echoHistoryOffset = 0;
  state.everyOtherSample = 1;
  state.echoOffset = 0;
  state.counter = 0;

  for(auto n : range(8)) {
    memory::fill(&voice[n], sizeof(Voice));
    voice[n].brrOffset = 1;
    voice[n].vbit = 1 << n;
    voice[n].vidx = n * 0x10;
  }

  for(auto r : range(0x80)) {
    REG(r) = random(0x00);
  }

  for(auto v : range(8)) {
    REG(v * 0x10 + ENVX) = 0;
    REG(v * 0x10 + OUTX) = 0;
  }

  REG(FLG) = 0xe0;
}

#undef REG
#undef VREG

}
