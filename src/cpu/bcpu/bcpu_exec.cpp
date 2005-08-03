inline void bCPU::cycle_edge() {
int c, n, z;
  if(status.dma_state != DMASTATE_STOP) {
    switch(status.dma_state) {
    case DMASTATE_INIT:
      status.dma_state = DMASTATE_DMASYNC;
      break;
    case DMASTATE_DMASYNC:
      n = 8 - dma_counter() + 8;
      add_cycles(n);
      status.dma_cycle_count = n;
      for(z=0;z<8;z++) {
        if(channel[z].active == false)continue;
        add_cycles(8);
        status.dma_cycle_count += 8;
        status.dma_cycle_count += channel[z].xfersize << 3;
      }
      status.cpu_state = CPUSTATE_DMA;
      status.dma_state = DMASTATE_RUN;
      while(status.dma_state == DMASTATE_RUN) {
        dma_run();
      }
      status.cpu_state = CPUSTATE_RUN;
      c = status.cycle_count;
      z = c - (status.dma_cycle_count % c);
      if(!z)z = c;
      add_cycles(z);
      status.dma_state = DMASTATE_STOP;
      break;
    }
  }
}

void bCPU::exec_opcode() {
  snes->notify(SNES::CPU_EXEC_OPCODE_BEGIN);
  (this->*optbl[op_read()])();
  snes->notify(SNES::CPU_EXEC_OPCODE_END);
}

void bCPU::init_op_tables() {
#include "bcpu_optable.cpp"
}
