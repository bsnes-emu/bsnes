//4194304hz (4 * 1024 * 1024)
//70224 clocks/frame
//  456 clocks/scanline
//  154 scanlines/frame

//4194304 /   4096 = 1024
//4194304 / 262144 =   16
//4194304 /  65536 =   64
//4394304 /  16384 =  256

#ifdef CPU_CPP

#include "opcode.cpp"

void CPU::add_clocks(unsigned clocks) {
  system.clocks_executed += clocks;
  scheduler.exit(Scheduler::ExitReason::StepEvent);

  status.clock += clocks;
  if(status.clock >= 4 * 1024 * 1024) {
    status.clock -= 4 * 1024 * 1024;
    cartridge.mbc3.second();
  }

  status.timer0 += clocks;
  if(status.timer0 >= 16) timer_stage0();

  cpu.clock += clocks;
  if(cpu.clock >= 0) co_switch(scheduler.active_thread = lcd.thread);
}

void CPU::timer_stage0() {  //262144hz
  if(status.timer_enable && status.timer_clock == 1) {
    if(++status.tima == 0) {
      status.tima = status.tma;
      interrupt_raise(Interrupt::Timer);
    }
  }

  status.timer0 -= 16;
  if(++status.timer1 >= 4) timer_stage1();
}

void CPU::timer_stage1() {  // 65536hz
  if(status.timer_enable && status.timer_clock == 2) {
    if(++status.tima == 0) {
      status.tima = status.tma;
      interrupt_raise(Interrupt::Timer);
    }
  }

  status.timer1 -= 4;
  if(++status.timer2 >= 4) timer_stage2();
}

void CPU::timer_stage2() {  // 16384hz
  if(status.timer_enable && status.timer_clock == 3) {
    if(++status.tima == 0) {
      status.tima = status.tma;
      interrupt_raise(Interrupt::Timer);
    }
  }

  status.div++;

  status.timer2 -= 4;
  if(++status.timer3 >= 2) timer_stage3();
}

void CPU::timer_stage3() {  //  8192hz
  if(status.serial_transfer && status.serial_clock) {
    if(--status.serial_bits == 0) {
      status.serial_transfer = 0;
      interrupt_raise(Interrupt::Serial);
    }
  }

  status.timer3 -= 2;
  if(++status.timer4 >= 2) timer_stage4();
}

void CPU::timer_stage4() {  //  4096hz
  if(status.timer_enable && status.timer_clock == 0) {
    if(++status.tima == 0) {
      status.tima = status.tma;
      interrupt_raise(Interrupt::Timer);
    }
  }

  status.timer4 -= 2;
}

#endif
