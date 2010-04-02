#ifdef SCPU_CPP

void sCPU::queue_event(unsigned id) {
  switch(id) {
    //interrupts triggered during (H)DMA do not trigger immediately after
    case EventIrqLockRelease: {
      status.irq_lock = false;
    } break;

    //S-CPU WRAM consists of two 64kbyte DRAM chips, which must be refreshed
    //once per scanline to avoid memory decay.
    case EventDramRefresh: {
      add_clocks(40);
    } break;

    //HDMA init routine; occurs once per frame
    case EventHdmaInit: {
      cycle_edge_state |= EventFlagHdmaInit;
    } break;

    //HDMA run routine; occurs once per scanline
    case EventHdmaRun: {
      cycle_edge_state |= EventFlagHdmaRun;
    } break;
  }
}

#endif
