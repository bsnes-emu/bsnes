#include <snes/snes.hpp>

#define ARMDSP_CPP
namespace SNES {

static bool trace = 0;

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

  //if(pipeline.instruction.address == 0x00000208) trace = 1;
    if(trace) {
      print("\n", disassemble_registers(), "\n");
      print(disassemble_opcode(pipeline.instruction.address), "\n");
      usleep(200000);
    }

    instruction = pipeline.instruction.opcode;
    if((instruction & 0x0fb000f0) == 0x01200000) { op_move_status_register_to_register(); continue; }
    if((instruction & 0x0e000010) == 0x00000000) { op_data_immediate_shift(); continue; }
    if((instruction & 0x0e000090) == 0x00000010) { op_data_register_shift(); continue; }
    if((instruction & 0x0e000000) == 0x02000000) { op_data_immediate(); continue; }
    if((instruction & 0x0e000000) == 0x04000000) { op_move_immediate_offset(); continue; }
    if((instruction & 0x0e000000) == 0x08000000) { op_move_multiple(); continue; }
    if((instruction & 0x0e000000) == 0x0a000000) { op_branch(); continue; }

    exception = true;
  }
}

uint8 ArmDSP::mmio_read(unsigned addr) {
  uint8 data = 0x00;
  switch(addr & 0xffff) {
  case 0x3800: data = w40000000; break;
  case 0x3802: data = r3802; break;
  case 0x3804: data = r3804; break;
  }

  print("* r", hex<6>(addr), " = ", hex<2>(data), "\n");
  usleep(200000);
  return data;
}

void ArmDSP::mmio_write(unsigned addr, uint8 data) {
  print("* w", hex<6>(addr), " = ", hex<2>(data), "\n");
  usleep(200000);

  switch(addr & 0xffff) {
  case 0x3800: w3800 = data; break;
  case 0x3802: w3802 = data; trace = 0; cputoarm = true; break;
  case 0x3804: w3804 = data; break;
  }
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

r3800=r3802=r3804=0;
w3800=w3802=w3804=0;
r40000000=r40000010=r40000020=r40000024=r40000028=r4000002c=0;
w40000000=w40000010=w40000020=w40000024=w40000028=w4000002c=0;
cputoarm=0;
armtocpu=0;
}

}
