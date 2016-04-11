auto DSP::calculateFIR(bool channel, int index) -> int {
  int sample = state.echoHistory[channel][(uint3)(state.echoHistoryOffset + index + 1)];
  return (sample * (int8)REG(FIR + index * 0x10)) >> 6;
}

auto DSP::echoOutput(bool channel) -> int {
  int output = (int16)((state._mainOut[channel] * (int8)REG(MVOLL + channel * 0x10)) >> 7)
                + (int16)((state._echoIn [channel] * (int8)REG(EVOLL + channel * 0x10)) >> 7);
  return sclamp<16>(output);
}

auto DSP::echoRead(bool channel) -> void {
  uint addr = state._echoPointer + channel * 2;
  uint8 lo = smp.apuram[(uint16)(addr + 0)];
  uint8 hi = smp.apuram[(uint16)(addr + 1)];
  int s = (int16)((hi << 8) + lo);
  state.echoHistory[channel][state.echoHistoryOffset] = s >> 1;
}

auto DSP::echoWrite(bool channel) -> void {
  if(!(state._echoDisabled & 0x20)) {
    uint addr = state._echoPointer + channel * 2;
    int s = state._echoOut[channel];
    smp.apuram[(uint16)(addr + 0)] = s;
    smp.apuram[(uint16)(addr + 1)] = s >> 8;
  }

  state._echoOut[channel] = 0;
}

auto DSP::echo22() -> void {
  //history
  state.echoHistoryOffset++;

  state._echoPointer = (uint16)((state._esa << 8) + state.echoOffset);
  echoRead(0);

  //FIR
  int l = calculateFIR(0, 0);
  int r = calculateFIR(1, 0);

  state._echoIn[0] = l;
  state._echoIn[1] = r;
}

auto DSP::echo23() -> void {
  int l = calculateFIR(0, 1) + calculateFIR(0, 2);
  int r = calculateFIR(1, 1) + calculateFIR(1, 2);

  state._echoIn[0] += l;
  state._echoIn[1] += r;

  echoRead(1);
}

auto DSP::echo24() -> void {
  int l = calculateFIR(0, 3) + calculateFIR(0, 4) + calculateFIR(0, 5);
  int r = calculateFIR(1, 3) + calculateFIR(1, 4) + calculateFIR(1, 5);

  state._echoIn[0] += l;
  state._echoIn[1] += r;
}

auto DSP::echo25() -> void {
  int l = state._echoIn[0] + calculateFIR(0, 6);
  int r = state._echoIn[1] + calculateFIR(1, 6);

  l = (int16)l;
  r = (int16)r;

  l += (int16)calculateFIR(0, 7);
  r += (int16)calculateFIR(1, 7);

  state._echoIn[0] = sclamp<16>(l) & ~1;
  state._echoIn[1] = sclamp<16>(r) & ~1;
}

auto DSP::echo26() -> void {
  //left output volumes
  //(save sample for next clock so we can output both together)
  state._mainOut[0] = echoOutput(0);

  //echo feedback
  int l = state._echoOut[0] + (int16)((state._echoIn[0] * (int8)REG(EFB)) >> 7);
  int r = state._echoOut[1] + (int16)((state._echoIn[1] * (int8)REG(EFB)) >> 7);

  state._echoOut[0] = sclamp<16>(l) & ~1;
  state._echoOut[1] = sclamp<16>(r) & ~1;
}

auto DSP::echo27() -> void {
  //output
  int outl = state._mainOut[0];
  int outr = echoOutput(1);
  state._mainOut[0] = 0;
  state._mainOut[1] = 0;

  //todo: global muting isn't this simple
  //(turns DAC on and off or something, causing small ~37-sample pulse when first muted)
  if(REG(FLG) & 0x40) {
    outl = 0;
    outr = 0;
  }

  //output sample to DAC
  audio.sample(outl, outr);
}

auto DSP::echo28() -> void {
  state._echoDisabled = REG(FLG);
}

auto DSP::echo29() -> void {
  state._esa = REG(ESA);

  if(!state.echoOffset) state.echoLength = (REG(EDL) & 0x0f) << 11;

  state.echoOffset += 4;
  if(state.echoOffset >= state.echoLength) state.echoOffset = 0;

  //write left echo
  echoWrite(0);

  state._echoDisabled = REG(FLG);
}

auto DSP::echo30() -> void {
  //write right echo
  echoWrite(1);
}
