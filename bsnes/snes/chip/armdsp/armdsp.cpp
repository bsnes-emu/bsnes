#include <snes/snes.hpp>

#define ARMDSP_CPP
namespace SNES {

#include "opcodes.cpp"
#include "memory.cpp"
#include "disassembler.cpp"
#include "serialization.cpp"
ArmDSP armdsp;

void ArmDSP::Enter() { armdsp.enter(); }

void ArmDSP::enter() {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    step(16);
    synchronize_cpu();

    print(disassemble_opcode(r[15]), "\n", disassemble_registers(), "\n");

    instruction = bus_read(r[15]);
    r[15] += 8;  //pipeline adjust
    if((instruction & 0x0e000000) == 0x0a000000) { op_branch(); continue; }
    if((instruction & 0x0e000000) == 0x02000000) { op_data_immediate(); r[15] -= 4; continue; }

    print("* ARM unknown instruction\n");
    while(true) {
      step(21477272);
      synchronize_cpu();
    }
  }
}

uint8 ArmDSP::mmio_read(unsigned addr) {
//print("* r", hex<4>(addr), "\n");
  return 0x00;
}

void ArmDSP::mmio_write(unsigned addr, uint8 data) {
  print("* w", hex<4>(addr), "w = ", hex<2>(data), "\n");
}

void ArmDSP::init() {
}

void ArmDSP::load() {
}

void ArmDSP::unload() {
}

void ArmDSP::power() {
}

void ArmDSP::reset() {
  create(ArmDSP::Enter, 21477272);

  for(auto &rd : r) rd = 0x00000000;
}

}
