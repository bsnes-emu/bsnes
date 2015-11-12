auto DSP::misc27() -> void {
  state._pmon = REG(PMON) & ~1;  //voice 0 doesn't support PMON
}

auto DSP::misc28() -> void {
  state._non = REG(NON);
  state._eon = REG(EON);
  state._dir = REG(DIR);
}

auto DSP::misc29() -> void {
  state.everyOtherSample ^= 1;
  if(state.everyOtherSample) {
    state.konBuffer &= ~state.kon;  //clears KON 63 clocks after it was last read
  }
}

auto DSP::misc30() -> void {
  if(state.everyOtherSample) {
    state.kon = state.konBuffer;
    state._koff = REG(KOFF);
  }

  counterTick();

  //noise
  if(counterPoll(REG(FLG) & 0x1f)) {
    signed feedback = (state.noise << 13) ^ (state.noise << 14);
    state.noise = (feedback & 0x4000) ^ (state.noise >> 1);
  }
}
