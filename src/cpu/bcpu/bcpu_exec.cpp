void bCPU::last_cycle() {
//DMV27: keep previous nmi value,
//to allow wai and irq to work properly
  time.nmi_pending = nmi_test() || time.nmi_pending;
  time.irq_pending = irq_test();
}

void bCPU::pre_exec_cycle() {
  if(!run_state.dma && !run_state.hdma)return;

int c, z;
  if(run_state.hdma) {
    switch(status.hdma_state) {
    case HDMASTATE_ICPUSYNC:
    case HDMASTATE_CPUSYNC:
      c = status.cycle_count;
      z = c - (status.hdma_cycle_count % c);
      if(!z)z = c;
      add_cycles(z);
      run_state.hdma = false;
      break;
    }
  }

  if(run_state.dma) {
    switch(status.dma_state) {
    case DMASTATE_CPUSYNC:
      c = status.cycle_count;
      z = c - (status.dma_cycle_count % c);
      if(!z)z = c;
      add_cycles(z);
      run_state.dma = false;
      break;
    }
  }
}

void bCPU::exec_hdma() {
int n;
static int z;
  switch(status.hdma_state) {
  case HDMASTATE_IDMASYNC:
    status.hdma_cycle_count = 0;
    z = 0;
    if(!run_state.dma) {
      exec_cycle();
      status.hdma_state = HDMASTATE_IDMASYNC2;
    } else {
      status.hdma_state = HDMASTATE_IDMASYNC3;
    }
    break;
  case HDMASTATE_IDMASYNC2:
    n = 8 - dma_counter() + 8;
    add_cycles(n);
    status.hdma_cycle_count += n;
    status.hdma_state = HDMASTATE_IDMASYNC3;
    break;
  case HDMASTATE_IDMASYNC3:
    channel[z].hdma_active = channel[z].hdma_enabled;
    if(channel[z].hdma_enabled) {
      channel[z].hdma_addr = channel[z].srcaddr;
      hdma_update(z); //updates status.hdma_cycle_count
    }
    if(++z < 8)break;
    if(!run_state.dma) {
      status.hdma_state = HDMASTATE_ICPUSYNC;
    } else {
      run_state.hdma = false;
    }
    break;
  case HDMASTATE_ICPUSYNC:
    exec_cycle();
    break;

  case HDMASTATE_DMASYNC:
    status.hdma_cycle_count = 0;
    z = 0;
    if(!run_state.dma) {
      exec_cycle();
      status.hdma_state = HDMASTATE_DMASYNC2;
    } else {
      status.hdma_state = HDMASTATE_DMASYNC3;
    }
    break;
  case HDMASTATE_DMASYNC2:
    n = 8 - dma_counter() + 8;
    add_cycles(n);
    status.hdma_cycle_count += n;
    status.hdma_state = HDMASTATE_DMASYNC3;
    break;
  case HDMASTATE_DMASYNC3:
    if(channel[z].hdma_active) {
      add_cycles(8);
      status.hdma_cycle_count += 8;
    }
    if(++z < 8)break;
    status.hdma_state = HDMASTATE_RUN;
    break;
  case HDMASTATE_RUN:
    hdma_run(); //updates status.hdma_cycle_count
    if(!run_state.dma) {
      status.hdma_state = HDMASTATE_CPUSYNC;
    } else {
      run_state.hdma = false;
    }
    break;
  case HDMASTATE_CPUSYNC:
    exec_cycle();
    break;
  }
}

void bCPU::exec_dma() {
int n;
static int z;
  switch(status.dma_state) {
  case DMASTATE_DMASYNC:
    exec_cycle();
    status.dma_state = DMASTATE_DMASYNC2;
    break;
  case DMASTATE_DMASYNC2:
    n = 8 - dma_counter() + 8;
    add_cycles(n);
    status.dma_cycle_count = n;
    z = 0;
    status.dma_state = DMASTATE_DMASYNC3;
    break;
  case DMASTATE_DMASYNC3:
    if(channel[z].active == true) {
      add_cycles(8);
      status.dma_cycle_count += 8;
    }
    if(++z < 8)break;
    status.dma_state = DMASTATE_RUN;
    break;
  case DMASTATE_RUN:
    dma_run(); //updates status.dma_cycle_count
    cycle_edge();
    break;
  case DMASTATE_CPUSYNC:
    exec_cycle();
    break;
  }
}

void bCPU::exec_cycle() {
//irq active? run one bus cycle of the irq event and return
  if(run_state.irq) {
    irq_run();
    return;
  }

  if(status.cycle_pos) {
    (this->*optbl[status.opcode])();
  #ifdef DEBUGGER
    if(status.cycle_pos == 0) {
      snes->notify(SNES::CPU_EXEC_OPCODE_END);
    }
  #endif
    return;
  }

//on first cycle?
#ifdef DEBUGGER
  snes->notify(SNES::CPU_EXEC_OPCODE_BEGIN);
#endif
  status.opcode = op_read();
  status.cycle_pos = 1;
}

//only return true when we are on an opcode edge
bool bCPU::in_opcode() {
  return (status.cycle_pos != 0);
}
