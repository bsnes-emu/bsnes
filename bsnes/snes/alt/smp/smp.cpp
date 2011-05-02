#include <snes/snes.hpp>

#define SMP_CPP
namespace SNES {

#if defined(DEBUGGER)
  #include "debugger/debugger.cpp"
  SMPDebugger smp;
#else
  SMP smp;
#endif

#include "algorithms.cpp"
#include "core.cpp"
#include "disassembler.cpp"
#include "iplrom.cpp"
#include "memory.cpp"
#include "timing.cpp"

void SMP::step(unsigned clocks) {
  clock += clocks * (uint64)cpu.frequency;
  dsp.clock -= clocks;
  synchronize_dsp();
}

void SMP::synchronize_cpu() {
  if(CPU::Threaded == true) {
    if(clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(cpu.thread);
  } else {
  //while(clock >= 0) cpu.enter();
  }
}

void SMP::synchronize_dsp() {
  if(DSP::Threaded == true) {
    if(dsp.clock < 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(dsp.thread);
  } else {
    while(dsp.clock < 0) dsp.enter();
  }
}

void SMP::enter() {
  op_step();
}

void SMP::power() {
  Processor::frequency = system.apu_frequency() / 24;
  Processor::clock = 0;

  timer0.target = 0;
  timer1.target = 0;
  timer2.target = 0;

  reset();
}

void SMP::reset() {
  for(unsigned n = 0x0000; n <= 0xffff; n++) apuram[n] = 0x00;

  regs.pc = 0xffc0;
  regs.sp = 0x00ef;
  regs.a = 0x00;
  regs.x = 0x00;
  regs.y = 0x00;
  regs.p = 0x02;

  //timing
  status.clock_counter = 0;
  status.dsp_counter = 0;
  status.timer_step = 3;

  //$00f0
  status.clock_speed = 0;
  status.timer_speed = 0;
  status.timers_enable = true;
  status.ram_disable = false;
  status.ram_writable = true;
  status.timers_disable = false;

  //$00f1
  status.iplrom_enable = true;

  //$00f2
  status.dsp_addr = 0x00;

  //$00f8,$00f9
  status.ram00f8 = 0x00;
  status.ram00f9 = 0x00;
}

void SMP::serialize(serializer &s) {
}

SMP::SMP() {
}

SMP::~SMP() {
}

}
