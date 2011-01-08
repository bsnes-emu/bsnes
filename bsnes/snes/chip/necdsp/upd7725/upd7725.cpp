#include <snes/snes.hpp>

#define UPD7725_CPP
namespace SNES {

uPD7725 upd7725;
uPD7725SR upd7725sr;
uPD7725DR upd7725dr;

void uPD7725::Enter() { upd7725.enter(); }

void uPD7725::enter() {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    uPDcore::exec();
    step(1);
    synchronize_cpu();
  }
}

uint8 uPD7725::read(bool mode) {
  cpu.synchronize_coprocessor();
  if(mode == 0) return regs.sr >> 8;

  if(regs.sr.drc == 0) {
    //16-bit
    if(regs.sr.drs == 0) {
      regs.sr.drs = 1;
      return regs.dr >> 0;
    } else {
      regs.sr.rqm = 0;
      regs.sr.drs = 0;
      return regs.dr >> 8;
    }
  } else {
    //8-bit
    regs.sr.rqm = 0;
    return regs.dr >> 0;
  }
}

void uPD7725::write(bool mode, uint8 data) {
  cpu.synchronize_coprocessor();
  if(mode == 0) return;

  if(regs.sr.drc == 0) {
    //16-bit
    if(regs.sr.drs == 0) {
      regs.sr.drs = 1;
      regs.dr = (regs.dr & 0xff00) | (data << 0);
    } else {
      regs.sr.rqm = 0;
      regs.sr.drs = 0;
      regs.dr = (data << 8) | (regs.dr & 0x00ff);
    }
  } else {
    //8-bit
    regs.sr.rqm = 0;
    regs.dr = (regs.dr & 0xff00) | (data << 0);
  }
}

void uPD7725::init() {
}

void uPD7725::enable() {
}

void uPD7725::power() {
  for(unsigned i = 0; i < 256; i++) dataRAM[i] = 0x0000;
  reset();
}

void uPD7725::reset() {
  create(uPD7725::Enter, 8 * 1024 * 1024);  //8.192MHz

  regs.pc = 0x000;
  regs.stack[0] = 0x000;
  regs.stack[1] = 0x000;
  regs.stack[2] = 0x000;
  regs.stack[3] = 0x000;
  regs.flaga = 0x00;
  regs.flagb = 0x00;
  regs.sr = 0x0000;
  regs.rp = 0x3ff;
  regs.siack = 0;
  regs.soack = 0;
}

void uPD7725::serialize(serializer &s) {
  Coprocessor::serialize(s);
  uPDcore::serialize(s);
}

uPD7725::uPD7725() : uPDcore(11, 10, 8) {
  //NEC uPD7725:
  //8.192 MIPS (8.192MHz / 1)
  //11-bit ProgramROM (2048 x 24-bit) w/4-level stack
  //10-bit DataROM    (1024 x 16-bit)
  // 8-bit DataRAM    ( 256 x 16-bit)
}

uint8 uPD7725SR::read(unsigned) { return upd7725.read(0); }
void uPD7725SR::write(unsigned, uint8 data) { upd7725.write(0, data); }

uint8 uPD7725DR::read(unsigned) { return upd7725.read(1); }
void uPD7725DR::write(unsigned, uint8 data) { upd7725.write(1, data); }

}
