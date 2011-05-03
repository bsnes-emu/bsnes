#include <snes/snes.hpp>

#define SMP_CPP
namespace SNES {

#if defined(DEBUGGER)
  #include "debugger/debugger.cpp"
  #include "debugger/disassembler.cpp"
  SMPDebugger smp;
#else
  SMP smp;
#endif

#include "algorithms.cpp"
#include "core.cpp"
#include "iplrom.cpp"
#include "memory.cpp"
#include "timing.cpp"

void SMP::synchronize_cpu() {
  if(CPU::Threaded == true) {
    if(clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(cpu.thread);
  } else {
    while(clock >= 0) cpu.enter();
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
  while(clock < 0) op_step();
}

void SMP::power() {
  Processor::frequency = system.apu_frequency();
  Processor::clock = 0;

  timer0.target = 0;
  timer1.target = 0;
  timer2.target = 0;

  for(unsigned n = 0; n < 256; n++) {
    cycle_table_dsp[n] = (cycle_count_table[n] * 24);
    cycle_table_cpu[n] = (cycle_count_table[n] * 24) * cpu.frequency;
  }

  reset();
}

void SMP::reset() {
  for(unsigned n = 0x0000; n <= 0xffff; n++) apuram[n] = 0x00;

  regs.pc = 0xffc0;
  regs.sp = 0xef;
  regs.a = 0x00;
  regs.x = 0x00;
  regs.y = 0x00;
  regs.p = 0x02;

  //timing
  status.clock_counter = 0;
  status.dsp_counter = 0;
  status.timer_step = 3;

  //$00f1
  status.iplrom_enable = true;

  //$00f2
  status.dsp_addr = 0x00;

  //$00f8,$00f9
  status.ram00f8 = 0x00;
  status.ram00f9 = 0x00;
}

void SMP::serialize(serializer &s) {
  Processor::serialize(s);

  s.array(apuram, 64 * 1024);

  s.integer(regs.pc);
  s.integer(regs.sp);
  s.integer(regs.a);
  s.integer(regs.x);
  s.integer(regs.y);

  s.integer(regs.p.n);
  s.integer(regs.p.v);
  s.integer(regs.p.p);
  s.integer(regs.p.b);
  s.integer(regs.p.h);
  s.integer(regs.p.i);
  s.integer(regs.p.z);
  s.integer(regs.p.c);

  s.integer(status.clock_counter);
  s.integer(status.dsp_counter);
  s.integer(status.timer_step);

  s.integer(status.iplrom_enable);

  s.integer(status.dsp_addr);

  s.integer(status.ram00f8);
  s.integer(status.ram00f9);
}

SMP::SMP() {
  apuram = new uint8[64 * 1024];
  stackram = apuram + 0x0100;
}

SMP::~SMP() {
}

}
