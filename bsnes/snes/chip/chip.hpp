struct Coprocessor : Processor {
  alwaysinline void step(unsigned clocks);
  alwaysinline void synchronize_cpu();
};

#include <chip/supergameboy/supergameboy.hpp>
#include <chip/superfx/superfx.hpp>
#include <chip/sa1/sa1.hpp>
#include <chip/upd77c25/upd77c25.hpp>
#include <chip/bsx/bsx.hpp>
#include <chip/srtc/srtc.hpp>
#include <chip/sdd1/sdd1.hpp>
#include <chip/spc7110/spc7110.hpp>
#include <chip/cx4/cx4.hpp>
#include <chip/obc1/obc1.hpp>
#include <chip/st0010/st0010.hpp>
#include <chip/st0011/st0011.hpp>
#include <chip/st0018/st0018.hpp>
#include <chip/msu1/msu1.hpp>
#include <chip/serial/serial.hpp>

void Coprocessor::step(unsigned clocks) {
  clock += clocks * (uint64)cpu.frequency;
}

void Coprocessor::synchronize_cpu() {
  if(clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(cpu.thread);
}
