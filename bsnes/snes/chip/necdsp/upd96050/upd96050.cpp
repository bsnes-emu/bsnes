#include <snes/snes.hpp>

#define UPD96050_CPP
namespace SNES {

uPD96050 upd96050;

void uPD96050::Enter() { upd96050.enter(); }

void uPD96050::enter() {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    uPDcore::exec();
    step(2);
    synchronize_cpu();
  }
}

uint8 uPD96050::read(unsigned addr) {
  cpu.synchronize_coprocessor();
  regs.sr.rqm = 0;

  bool hi = addr & 1;
  addr = (addr >> 1) & 2047;

  if(hi == false) {
    return dataRAM[addr] >> 0;
  } else {
    return dataRAM[addr] >> 8;
  }
}

void uPD96050::write(unsigned addr, uint8 data) {
  cpu.synchronize_coprocessor();
  regs.sr.rqm = 0;

  bool hi = addr & 1;
  addr = (addr >> 1) & 2047;

  if(hi == false) {
    dataRAM[addr] = (dataRAM[addr] & 0xff00) | (data << 0);
  } else {
    dataRAM[addr] = (dataRAM[addr] & 0x00ff) | (data << 8);
  }
}

void uPD96050::init() {
}

void uPD96050::enable() {
}

void uPD96050::power() {
  for(unsigned i = 0; i < 2048; i++) dataRAM[i] = 0x0000;
  reset();
}

void uPD96050::reset() {
  create(uPD96050::Enter, 20 * 1000 * 1000);  //20MHz

  regs.pc = 0x0000;
  regs.stack[0] = 0x0000;
  regs.stack[1] = 0x0000;
  regs.stack[2] = 0x0000;
  regs.stack[3] = 0x0000;
  regs.stack[4] = 0x0000;
  regs.stack[5] = 0x0000;
  regs.stack[6] = 0x0000;
  regs.stack[7] = 0x0000;
  regs.flaga = 0x00;
  regs.flagb = 0x00;
  regs.sr = 0x0000;
  regs.rp = 0x03ff;
  regs.siack = 0;
  regs.soack = 0;
}

void uPD96050::serialize(serializer &s) {
  Coprocessor::serialize(s);
  uPDcore::serialize(s);
}

uPD96050::uPD96050() : uPDcore(14, 11, 11) {
  //NEC uPD96050:
  //10MIPS (20MHz / 2)
  //14-bit ProgramROM (16384 x 24-bit) w/8-level stack
  //11-bit DataROM    ( 2048 x 16-bit)
  //11-bit DataRAM    ( 2048 x 16-bit) w/battery backup
}

}
