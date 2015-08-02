struct Coprocessor : Thread {
  alwaysinline auto step(unsigned clocks) -> void;
  alwaysinline auto synchronize_cpu() -> void;
};

#include <sfc/chip/icd2/icd2.hpp>
#include <sfc/chip/mcc/mcc.hpp>
#include <sfc/chip/nss/nss.hpp>
#include <sfc/chip/event/event.hpp>

#include <sfc/chip/sa1/sa1.hpp>
#include <sfc/chip/superfx/superfx.hpp>

#include <sfc/chip/armdsp/armdsp.hpp>
#include <sfc/chip/hitachidsp/hitachidsp.hpp>
#include <sfc/chip/necdsp/necdsp.hpp>

#include <sfc/chip/epsonrtc/epsonrtc.hpp>
#include <sfc/chip/sharprtc/sharprtc.hpp>

#include <sfc/chip/spc7110/spc7110.hpp>
#include <sfc/chip/sdd1/sdd1.hpp>
#include <sfc/chip/obc1/obc1.hpp>

#include <sfc/chip/msu1/msu1.hpp>

auto Coprocessor::step(unsigned clocks) -> void {
  clock += clocks * (uint64)cpu.frequency;
}

auto Coprocessor::synchronize_cpu() -> void {
  if(clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(cpu.thread);
}
