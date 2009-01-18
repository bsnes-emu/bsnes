void export_memory() {
  file fp;

  fp.open(Cartridge::filepath("wram.bin", snes.config.path.exportdata), file::mode_write);
  for(unsigned i = 0; i < memory::wram.size(); i++) fp.write(memory::wram[i]);
  fp.close();

  fp.open(Cartridge::filepath("apuram.bin", snes.config.path.exportdata), file::mode_write);
  for(unsigned i = 0; i < memory::apuram.size(); i++) fp.write(memory::apuram[i]);
  fp.close();

  fp.open(Cartridge::filepath("vram.bin", snes.config.path.exportdata), file::mode_write);
  for(unsigned i = 0; i < memory::vram.size(); i++) fp.write(memory::vram[i]);
  fp.close();

  fp.open(Cartridge::filepath("oam.bin", snes.config.path.exportdata), file::mode_write);
  for(unsigned i = 0; i < memory::oam.size(); i++) fp.write(memory::oam[i]);
  fp.close();

  fp.open(Cartridge::filepath("cgram.bin", snes.config.path.exportdata), file::mode_write);
  for(unsigned i = 0; i < memory::cgram.size(); i++) fp.write(memory::cgram[i]);
  fp.close();

  status.flush();
  status.enqueue(translate["Memory exported."]);
}

void toggle_cputrace() {
  if(!tracer.enabled()) tracer.enable(true);

  bool state = !tracer.cpuop_enabled();
  tracer.cpuop_enable(state);
  status.flush();
  status.enqueue(state
  ? translate["S-CPU tracing enabled."]
  : translate["S-CPU tracing disabled."]
  );
}

void toggle_cputracemask() {
  bool state = !tracer.cpuopmask_enabled();
  tracer.cpuopmask_enable(state);
  status.flush();
  status.enqueue(state
  ? translate["S-CPU trace mask enabled."]
  : translate["S-CPU trace mask disabled."]
  );
}

void toggle_smptrace() {
  if(!tracer.enabled()) tracer.enable(true);

  bool state = !tracer.smpop_enabled();
  tracer.smpop_enable(state);
  status.flush();
  status.enqueue(state
  ? translate["S-SMP tracing enabled."]
  : translate["S-SMP tracing disabled."]
  );
}

void toggle_smptracemask() {
  bool state = !tracer.smpopmask_enabled();
  tracer.smpopmask_enable(state);
  status.flush();
  status.enqueue(state
  ? translate["S-SMP trace mask enabled."]
  : translate["S-SMP trace mask disabled."]
  );
}
