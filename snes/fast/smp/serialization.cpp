#ifdef SMP_CPP

static void smp_state_save(unsigned char **out, void *in, size_t size) {
  memcpy(*out, in, size);
  *out += size;
}

static void smp_state_load(unsigned char **in, void *out, size_t size) {
  memcpy(out, *in, size);
  *in += size;
}

void SMP::serialize(serializer &s) {
  Processor::serialize(s);
  s.integer(snes_spc_time);
  s.array(samplebuffer);

  unsigned char state[SNES_SPC::state_size];
  unsigned char *p = state;
  memset(&state, 0, SNES_SPC::state_size);
  if(s.mode() == serializer::Save) {
    snes_spc.copy_state(&p, smp_state_save);
    s.array(state);
  } else if(s.mode() == serializer::Load) {
    s.array(state);
    snes_spc.copy_state(&p, smp_state_load);
  } else {
    s.array(state);
  }
}

#endif
