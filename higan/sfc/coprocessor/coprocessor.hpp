struct Coprocessor : Thread {
  alwaysinline auto step(uint clocks) -> void;
  alwaysinline auto synchronizeCPU() -> void;
};

#include <sfc/coprocessor/icd2/icd2.hpp>
#include <sfc/coprocessor/mcc/mcc.hpp>
#include <sfc/coprocessor/nss/nss.hpp>
#include <sfc/coprocessor/event/event.hpp>

#include <sfc/coprocessor/sa1/sa1.hpp>
#include <sfc/coprocessor/superfx/superfx.hpp>

#include <sfc/coprocessor/armdsp/armdsp.hpp>
#include <sfc/coprocessor/hitachidsp/hitachidsp.hpp>
#include <sfc/coprocessor/necdsp/necdsp.hpp>

#include <sfc/coprocessor/epsonrtc/epsonrtc.hpp>
#include <sfc/coprocessor/sharprtc/sharprtc.hpp>

#include <sfc/coprocessor/spc7110/spc7110.hpp>
#include <sfc/coprocessor/sdd1/sdd1.hpp>
#include <sfc/coprocessor/obc1/obc1.hpp>

#include <sfc/coprocessor/msu1/msu1.hpp>

auto Coprocessor::step(uint clocks) -> void {
  clock += clocks * (uint64)cpu.frequency;
}

auto Coprocessor::synchronizeCPU() -> void {
  if(clock >= 0 && !scheduler.synchronizing()) co_switch(cpu.thread);
}
