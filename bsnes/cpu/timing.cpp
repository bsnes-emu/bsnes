#ifdef CPU_CPP

struct QueueEvent {
  enum : unsigned {
    DramRefresh,
    HdmaRun,
  };
};

void CPU::queue_event(unsigned id) {
  switch(id) {
    case QueueEvent::DramRefresh: return add_clocks(40);
    case QueueEvent::HdmaRun: return hdma_run();
  }
}

void CPU::last_cycle() {
  if(status.nmi_transition) {
    regs.wai = false;
    status.nmi_transition = false;
    status.nmi_pending = true;
  }

  if(status.irq_transition) {
    regs.wai = false;
    status.irq_transition = false;
    status.irq_pending = !regs.p.i;
  }
}

void CPU::add_clocks(unsigned clocks) {
  step(clocks);
  queue.tick(clocks);
  unsigned clocksleft = lineclocks() - hcounter();
  if(clocks > clocksleft) {
    add_time(clocksleft);
    add_time(clocks - clocksleft);
  } else {
    add_time(clocks);
  }
}

void CPU::add_time(unsigned clocks) {
  if(status.irq_line && (status.virq_enabled || status.hirq_enabled)) {
    status.irq_transition = true;
  }

  if(status.virq_enabled && !status.hirq_enabled) {
    bool irq_valid = status.irq_valid;
    status.irq_valid = vcounter() == status.vtime;
    if(!irq_valid && status.irq_valid) {
      status.irq_line = true;
      status.irq_transition = true;
    }
  } else if(status.hirq_enabled) {
    bool irq_valid = status.irq_valid;
    status.irq_valid = hcounter() <= status.htime * 4 && hcounter() + clocks > status.htime * 4;
    if(status.virq_enabled && vcounter() != status.vtime) status.irq_valid = false;
    if(!irq_valid && status.irq_valid) {
      status.irq_line = true;
      status.irq_transition = true;
    }
  }

  tick(clocks);
}

void CPU::scanline() {
  synchronize_smp();
  synchronize_ppu();
  synchronize_coprocessor();
  system.scanline();

  if(vcounter() == 0) hdma_init();

  queue.enqueue(534, QueueEvent::DramRefresh);

  if(vcounter() <= (ppu.overscan() == false ? 224 : 239)) {
    queue.enqueue(1104, QueueEvent::HdmaRun);
  }

  bool nmi_valid = status.nmi_valid;
  status.nmi_valid = vcounter() >= (ppu.overscan() == false ? 225 : 240);
  if(!nmi_valid && status.nmi_valid) {
    status.nmi_line = true;
    if(status.nmi_enabled) status.nmi_transition = true;
  } else if(nmi_valid && !status.nmi_valid) {
    status.nmi_line = false;
  }

  if(status.auto_joypad_poll_enabled && vcounter() == (ppu.overscan() == false ? 227 : 242)) {
    input.poll();
    run_auto_joypad_poll();
  }
}

void CPU::run_auto_joypad_poll() {
  uint16 joy1 = 0, joy2 = 0, joy3 = 0, joy4 = 0;
  for(unsigned i = 0; i < 16; i++) {
    uint8 port0 = input.port_read(0);
    uint8 port1 = input.port_read(1);

    joy1 |= (port0 & 1) ? (0x8000 >> i) : 0;
    joy2 |= (port1 & 1) ? (0x8000 >> i) : 0;
    joy3 |= (port0 & 2) ? (0x8000 >> i) : 0;
    joy4 |= (port1 & 2) ? (0x8000 >> i) : 0;
  }

  status.joy1l = joy1;
  status.joy1h = joy1 >> 8;

  status.joy2l = joy2;
  status.joy2h = joy2 >> 8;

  status.joy3l = joy3;
  status.joy3h = joy3 >> 8;

  status.joy4l = joy4;
  status.joy4h = joy4 >> 8;
}

#endif
