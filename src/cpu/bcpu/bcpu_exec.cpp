inline void bCPU::cycle_edge() {
int c, n, z;
  if(status.dma_state != DMASTATE_STOP) {
    switch(status.dma_state) {
    case DMASTATE_DMASYNC:
      status.dma_state = DMASTATE_DMASYNC2;
      break;
    case DMASTATE_DMASYNC2:
      n = 8 - dma_counter() + 8;
      add_cycles(n);
      status.dma_cycle_count = n;
      for(z=0;z<8;z++) {
        if(channel[z].active == false)continue;
        add_cycles(8);
        status.dma_cycle_count += 8;
      }
      status.cpu_state = CPUSTATE_DMA;
      status.dma_state = DMASTATE_RUN;
      break;
    case DMASTATE_RUN:
      status.dma_cycle_count += 8;
      break;
    case DMASTATE_CPUSYNC:
      status.cpu_state = CPUSTATE_RUN;
      status.dma_state = DMASTATE_CPUSYNC2;
      break;
    case DMASTATE_CPUSYNC2:
      c = status.cycle_count;
      z = c - (status.dma_cycle_count % c);
      if(!z)z = c;
      add_cycles(z);
      status.dma_state = DMASTATE_STOP;
      break;
    }
  }
}

void bCPU::exec_cycle() {
  if(status.cycle_pos == 0) {
    snes->notify(SNES::CPU_EXEC_OPCODE_BEGIN);
    status.opcode = op_read();
    status.cycle_pos = 1;
  } else {
    (this->*optbl[status.opcode])();
    if(status.cycle_pos == 0) {
      snes->notify(SNES::CPU_EXEC_OPCODE_END);
    }
  }
}

//only return true when we are on an opcode edge
bool bCPU::in_opcode() {
  return (status.cycle_pos != 0);
}

void bCPU::init_op_tables() {
#include "bcpu_optable.cpp"
}
