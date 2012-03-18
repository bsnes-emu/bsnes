void Scheduler::swapto(Processor &p) {
  active_thread = p.thread;
  co_switch(active_thread);
}

void Scheduler::synchronize_cpu() {
  if(sync == SynchronizeMode::CPU) {
    sync = SynchronizeMode::All;
    exit(ExitReason::SynchronizeEvent);
  }
}

void Scheduler::synchronize_all() {
  if(sync == SynchronizeMode::All) {
    exit(ExitReason::SynchronizeEvent);
  }
}
