#ifdef SMP_CPP

bool SMPDebugger::property(unsigned id, string &name, string &value) {
  unsigned n = 0;

  //$00f0
  if(id == n++) { name = "$00f0"; value = ""; return true; }
  if(id == n++) { name = "Clock Speed"; value = clock_speed(); return true; }
  if(id == n++) { name = "MMIO Disable"; value = mmio_disable(); return true; }
  if(id == n++) { name = "RAM Writable"; value = ram_writable(); return true; }

  //$00f1
  if(id == n++) { name = "$00f1"; value = ""; return true; }
  if(id == n++) { name = "IPLROM Enable"; value = iplrom_enable(); return true; }

  //$00f2
  if(id == n++) { name = "$00f2"; value = ""; return true; }
  if(id == n++) { name = "DSP Address"; value = sprint("0x$", strhex<2>(dsp_address())); return true; }

  return false;
}

#endif
