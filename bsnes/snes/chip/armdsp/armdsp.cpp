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

    step(4);  //todo: how many cycles per instruction?
    synchronize_cpu();

    if(exception) {
      print("* ARM unknown instruction\n");
      print("\n", disassemble_registers());
      print("\n", disassemble_opcode(pipeline.instruction.address), "\n");
      while(true) {
        step(21477272);
        synchronize_cpu();
      }
    }

    if(pipeline.reload) {
      pipeline.reload = false;
      pipeline.prefetch.address = r[15];
      pipeline.prefetch.opcode = bus_read<4>(r[15]);
      r[15].step();
    }

    pipeline.instruction = pipeline.prefetch;
    pipeline.prefetch.address = r[15];
    pipeline.prefetch.opcode = bus_read<4>(r[15]);
    r[15].step();

  //print("\n", disassemble_registers(), "\n");
  //print(disassemble_opcode(pipeline.instruction.address), "\n");

    instruction = pipeline.instruction.opcode;
    if((instruction & 0x0fb000f0) == 0x01200000) { op_move_status_register_to_register(); continue; }
    if((instruction & 0x0e000010) == 0x00000000) { op_data_immediate_shift(); continue; }
    if((instruction & 0x0e000000) == 0x02000000) { op_data_immediate(); continue; }
    if((instruction & 0x0e000000) == 0x04000000) { op_move_immediate_offset(); continue; }
    if((instruction & 0x0e000000) == 0x08000000) { op_move_multiple(); continue; }
    if((instruction & 0x0e000000) == 0x0a000000) { op_branch(); continue; }

    exception = true;
  }
}

uint8 ArmDSP::mmio_read(unsigned addr) {
  addr &= 0xffff;
  if(addr == 0x3800) return armport[0];
  if(addr == 0x3802) return armport[1];
  if(addr == 0x3804) return armport[3];

  print("* r", hex<4>(addr), "\n");
  return 0x00;
}

void ArmDSP::mmio_write(unsigned addr, uint8 data) {
  addr &= 0xffff;
  if(addr == 0x3802) cpuport[1] = data;
  if(addr == 0x3804) cpuport[0] = data;

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

  for(auto &rd : r) rd = 0;
  exception = false;

  pipeline.reload = true;

  r[15].write = [&] { pipeline.reload = true; };
}

}
