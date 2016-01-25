#include <sfc/sfc.hpp>

namespace SuperFamicom {

DSP dsp;
#include "audio.cpp"

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
  static_assert(sizeof(signed) >= 32 / 8, "signed >= 32-bits");
  static_assert((int8)0x80 == -0x80,      "8-bit sign extension");
  static_assert((int16)0x8000 == -0x8000, "16-bit sign extension");
  static_assert((uint16)0xffff0000 == 0,  "16-bit unsigned clip");
  static_assert((-1 >> 1) == -1,          "arithmetic shift right");

  //-0x8000 <= n <= +0x7fff
  assert(sclamp<16>(+0x8000) == +0x7fff);
  assert(sclamp<16>(-0x8001) == -0x8000);
}

/* timing */

auto DSP::step(unsigned clocks) -> void {
  clock += clocks;
}

auto DSP::synchronizeSMP() -> void {
  if(SMP::Threaded == true) {
    if(clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(smp.thread);
  } else {
    while(clock >= 0) smp.enter();
  }
}

auto DSP::Enter() -> void { dsp.enter(); }

auto DSP::enter() -> void {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

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
}

auto DSP::tick() -> void {
  step(3 * 8);
  synchronizeSMP();
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

auto DSP::power() -> void {
  for(auto& r : state.regs) r = 0;
  state.echoHistoryOffset = 0;
  state.everyOtherSample = false;
  state.kon = 0;
  state.noise = 0;
  state.counter = 0;
  state.echoOffset = 0;
  state.echoLength = 0;
  state.konBuffer = 0;
  state.endxBuffer = 0;
  state.envxBuffer = 0;
  state.outxBuffer = 0;
  state._pmon = 0;
  state._non = 0;
  state._eon = 0;
  state._dir = 0;
  state._koff = 0;
  state._brrNextAddress = 0;
  state._adsr0 = 0;
  state._brrHeader = 0;
  state._brrByte = 0;
  state._srcn = 0;
  state._esa = 0;
  state._echoDisabled = 0;
  state._dirAddress = 0;
  state._pitch = 0;
  state._output = 0;
  state._looped = 0;
  state._echoPointer = 0;
  state._mainOut[0] = state._mainOut[1] = 0;
  state._echoOut[0] = state._echoOut[1] = 0;
  state._echoIn[0] = state._echoIn[1] = 0;

  for(auto n : range(8)) {
    voice[n].bufferOffset = 0;
    voice[n].gaussianOffset = 0;
    voice[n].brrAddress = 0;
    voice[n].brrOffset = 1;
    voice[n].vbit = 1 << n;
    voice[n].vidx = n * 0x10;
    voice[n].konDelay = 0;
    voice[n].envelopeMode = EnvelopeRelease;
    voice[n].envelope = 0;
    voice[n].hiddenEnvelope = 0;
    voice[n]._envxOut = 0;
  }

  audio.coprocessorEnable(false);
}

auto DSP::reset() -> void {
  create(Enter, system.apuFrequency());

  REG(FLG) = 0xe0;
  state.noise = 0x4000;
  state.echoHistoryOffset = 0;
  state.everyOtherSample = 1;
  state.echoOffset = 0;
  state.counter = 0;
}

#undef REG
#undef VREG

}
