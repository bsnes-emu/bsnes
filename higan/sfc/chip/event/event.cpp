#include <sfc/sfc.hpp>

#define EVENT_CPP
namespace SuperFamicom {

Event event;

void Event::Enter() { event.enter(); }

void Event::enter() {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }
  }

  step(1);
  synchronize_cpu();
}

void Event::init() {
}

void Event::load() {
}

void Event::unload() {
  rom[0].reset();
  rom[1].reset();
  rom[2].reset();
  rom[3].reset();
  ram.reset();
}

void Event::power() {
}

void Event::reset() {
  create(Event::Enter, 1);
}

//DSP-1
uint8 Event::sr(unsigned addr) {
  return 0;
}

//DSP-1
void Event::dr(unsigned addr, uint8 data) {
}

//is there bank-switching?
uint8 Event::rom_read(unsigned addr) {
  return cpu.regs.mdr;
}

//is there read-protection?
uint8 Event::ram_read(unsigned addr) {
  return cpu.regs.mdr;
}

//is there write-protection?
void Event::ram_write(unsigned addr, uint8 data) {
}

void Event::serialize(serializer &s) {
  Thread::serialize(s);
  s.array(ram.data(), ram.size());
}

}
