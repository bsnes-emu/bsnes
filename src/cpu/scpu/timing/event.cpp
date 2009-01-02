#ifdef SCPU_CPP

void sCPU::queue_event(unsigned id) {
  switch(id) {
    case EventIrqLockRelease: {
      status.irq_lock = false;
    } break;

    case EventAluLockRelease: {
      status.alu_lock = false;
    } break;

    case EventDramRefresh: {
      add_clocks(40);
    } break;

    case EventHdmaInit: {
      event.cycle_edge |= EventFlagHdmaInit;
    } break;

    case EventHdmaRun: {
      event.cycle_edge |= EventFlagHdmaRun;
    } break;
  }
}

#endif  //ifdef SCPU_CPP
