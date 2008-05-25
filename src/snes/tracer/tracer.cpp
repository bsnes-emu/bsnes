#ifdef SNES_CPP

Tracer tracer;

void tprintf(const char *s, ...) {
  if(tracer.enabled() == false) { return; }

char str[4096];
va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);
  fprintf(tracer.fp, "%s\r\n", str);
}

void Tracer::trace_cpuop() {
  if(enabled() == false) { return; }
  if(cpuop_enabled() == false) { return; }
  if(cpu.in_opcode() == true) { return; }

  if(cpuopmask_enabled() == true) {
  uint addr = cpu.regs.pc.d;
    if(settings.cpuopmasktbl[addr >> 3] & 0x80 >> (addr & 7)) { return; }
    settings.cpuopmasktbl[addr >> 3] |= 0x80 >> (addr & 7);
  }

char t[1024];
  cpu.disassemble_opcode(t);
  fprintf(fp, "%s\r\n", t);
}

void Tracer::trace_smpop() {
  if(enabled() == false) { return; }
  if(smpop_enabled() == false) { return; }
  if(smp.in_opcode() == true) { return; }

  if(smpopmask_enabled() == true) {
  uint addr = smp.regs.pc;
    if(settings.smpopmasktbl[addr >> 3] & 0x80 >> (addr & 7)) { return; }
    settings.smpopmasktbl[addr >> 3] |= 0x80 >> (addr & 7);
  }

char t[1024];
  smp.disassemble_opcode(t);
  fprintf(fp, "%s\r\n", t);
}

void Tracer::enable(bool en) {
  if(en == true && enabled() == false) {
    fp = fopen("trace.log", "wb");
  } else if(en == false && enabled() == true) {
    fclose(fp);
    fp = 0;
  }

  settings.enabled = en;
}

void Tracer::cpuopmask_enable(bool en) {
  if(en == true && cpuopmask_enabled() == false) {
    settings.cpuopmasktbl = new(zeromemory) uint8_t[0x200000];
  } else if(en == false && cpuopmask_enabled() == true) {
    delete[] settings.cpuopmasktbl;
  }

  settings.cpuopmask = en;
}

void Tracer::smpopmask_enable(bool en) {
  if(en == true && smpopmask_enabled() == false) {
    settings.smpopmasktbl = new(zeromemory) uint8_t[0x2000];
  } else if(en == false && smpopmask_enabled() == true) {
    delete[] settings.smpopmasktbl;
  }

  settings.smpopmask = en;
}

Tracer::Tracer() {
  fp = 0;

  settings.cpuop        = false;
  settings.cpuopmask    = false;
  settings.cpuopmasktbl = 0;
  settings.cpudma       = false;

  settings.smpop        = false;
  settings.smpopmask    = false;
  settings.smpopmasktbl = 0;
}

Tracer::~Tracer() {
}

#endif //ifdef SNES_CPP
