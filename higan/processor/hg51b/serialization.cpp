auto HG51B::serialize(serializer& s) -> void {
  s.array(programRAM[0]);
  s.array(programRAM[1]);
  s.array(dataRAM);

  s.integer(r.p);
  s.integer(r.pb);
  s.integer(r.pc);

  s.boolean(r.n);
  s.boolean(r.z);
  s.boolean(r.c);
  s.boolean(r.i);

  s.integer(r.a);
  s.integer(r.acch);
  s.integer(r.accl);
  s.integer(r.busData);
  s.integer(r.romData);
  s.integer(r.ramData);
  s.integer(r.busAddress);
  s.integer(r.ramAddress);
  s.array(r.gpr);

  s.integer(io.lock);
  s.integer(io.halt);
  s.integer(io.irq);
  s.integer(io.rom);
  s.array(io.vector);

  s.integer(io.wait.rom);
  s.integer(io.wait.ram);

  s.integer(io.suspend.enable);
  s.integer(io.suspend.duration);

  s.integer(io.cache.enable);
  s.integer(io.cache.page);
  s.array(io.cache.lock);
  s.array(io.cache.address);
  s.integer(io.cache.base);
  s.integer(io.cache.pb);
  s.integer(io.cache.pc);

  s.integer(io.dma.enable);
  s.integer(io.dma.source);
  s.integer(io.dma.target);
  s.integer(io.dma.length);

  s.integer(io.bus.enable);
  s.integer(io.bus.reading);
  s.integer(io.bus.writing);
  s.integer(io.bus.pending);
  s.integer(io.bus.address);

  s.array(stack);
  s.integer(opcode);
}
