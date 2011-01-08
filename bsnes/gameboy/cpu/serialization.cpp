#ifdef CPU_CPP

void CPU::serialize(serializer &s) {
  s.array(wram);
  s.array(hram);

  s.integer(r.a.data);
  s.integer(r.f.z);
  s.integer(r.f.n);
  s.integer(r.f.h);
  s.integer(r.f.c);
  s.integer(r.b.data);
  s.integer(r.c.data);
  s.integer(r.d.data);
  s.integer(r.e.data);
  s.integer(r.h.data);
  s.integer(r.l.data);
  s.integer(r.sp.data);
  s.integer(r.pc.data);

  s.integer(status.clock);
  s.integer(status.halt);
  s.integer(status.stop);

  s.integer(status.ime);
  s.integer(status.timer0);
  s.integer(status.timer1);
  s.integer(status.timer2);
  s.integer(status.timer3);

  s.integer(status.p15);
  s.integer(status.p14);
  s.integer(status.joyp);
  s.integer(status.mlt_req);

  s.integer(status.div);
  s.integer(status.tima);
  s.integer(status.tma);
  s.integer(status.timer_enable);
  s.integer(status.timer_clock);

  s.integer(status.interrupt_request_joypad);
  s.integer(status.interrupt_request_serial);
  s.integer(status.interrupt_request_timer);
  s.integer(status.interrupt_request_stat);
  s.integer(status.interrupt_request_vblank);

  s.integer(status.interrupt_enable_joypad);
  s.integer(status.interrupt_enable_serial);
  s.integer(status.interrupt_enable_timer);
  s.integer(status.interrupt_enable_stat);
  s.integer(status.interrupt_enable_vblank);
}

#endif
