struct Coprocessor : Processor {
  alwaysinline void step(unsigned clocks);
  alwaysinline void synchronize_cpu();
};

#include <snes/chip/supergameboy/supergameboy.hpp>
#include <snes/chip/superfx/superfx.hpp>
#include <snes/chip/sa1/sa1.hpp>
#include <snes/chip/bsx/bsx.hpp>
#include <snes/chip/srtc/srtc.hpp>
#include <snes/chip/sdd1/sdd1.hpp>
#include <snes/chip/spc7110/spc7110.hpp>
#include <snes/chip/cx4/cx4.hpp>
#include <snes/chip/dsp1/dsp1.hpp>
#include <snes/chip/dsp2/dsp2.hpp>
#include <snes/chip/dsp3/dsp3.hpp>
#include <snes/chip/dsp4/dsp4.hpp>
#include <snes/chip/obc1/obc1.hpp>
#include <snes/chip/st0010/st0010.hpp>
#include <snes/chip/st0011/st0011.hpp>
#include <snes/chip/st0018/st0018.hpp>
#include <snes/chip/msu1/msu1.hpp>
#include <snes/chip/serial/serial.hpp>

void Coprocessor::step(unsigned clocks) {
  clock += clocks * (uint64)cpu.frequency;
}

void Coprocessor::synchronize_cpu() {
  if(clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(cpu.thread);
}
