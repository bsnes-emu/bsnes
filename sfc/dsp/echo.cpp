auto DSP::calculateFIR(signed i, bool channel) -> signed {
  signed s = state.echoHistory[channel][state.echoHistoryOffset + i + 1];
  return (s * (int8)REG(FIR + i * 0x10)) >> 6;
}

auto DSP::echoOutput(bool channel) -> signed {
  signed output = (int16)((state._mainOut[channel] * (int8)REG(MVOLL + channel * 0x10)) >> 7)
                + (int16)((state._echoIn [channel] * (int8)REG(EVOLL + channel * 0x10)) >> 7);
  return sclamp<16>(output);
}

auto DSP::echoRead(bool channel) -> void {
  unsigned addr = state._echoPointer + channel * 2;
  uint8 lo = smp.apuram[(uint16)(addr + 0)];
  uint8 hi = smp.apuram[(uint16)(addr + 1)];
  signed s = (int16)((hi << 8) + lo);
  state.echoHistory[channel].write(state.echoHistoryOffset, s >> 1);
}

auto DSP::echoWrite(bool channel) -> void {
  if(!(state._echoDisabled & 0x20)) {
    unsigned addr = state._echoPointer + channel * 2;
    signed s = state._echoOut[channel];
    smp.apuram[(uint16)(addr + 0)] = s;
    smp.apuram[(uint16)(addr + 1)] = s >> 8;
  }

  state._echoOut[channel] = 0;
}

auto DSP::echo22() -> void {
  //history
  state.echoHistoryOffset++;
  if(state.echoHistoryOffset >= EchoHistorySize) state.echoHistoryOffset = 0;

  state._echoPointer = (uint16)((state._esa << 8) + state.echoOffset);
  echoRead(0);

  //FIR
  signed l = calculateFIR(0, 0);
  signed r = calculateFIR(0, 1);

  state._echoIn[0] = l;
  state._echoIn[1] = r;
}

auto DSP::echo23() -> void {
  signed l = calculateFIR(1, 0) + calculateFIR(2, 0);
  signed r = calculateFIR(1, 1) + calculateFIR(2, 1);

  state._echoIn[0] += l;
  state._echoIn[1] += r;

  echoRead(1);
}

auto DSP::echo24() -> void {
  signed l = calculateFIR(3, 0) + calculateFIR(4, 0) + calculateFIR(5, 0);
  signed r = calculateFIR(3, 1) + calculateFIR(4, 1) + calculateFIR(5, 1);

  state._echoIn[0] += l;
  state._echoIn[1] += r;
}

auto DSP::echo25() -> void {
  signed l = state._echoIn[0] + calculateFIR(6, 0);
  signed r = state._echoIn[1] + calculateFIR(6, 1);

  l = (int16)l;
  r = (int16)r;

  l += (int16)calculateFIR(7, 0);
  r += (int16)calculateFIR(7, 1);

  state._echoIn[0] = sclamp<16>(l) & ~1;
  state._echoIn[1] = sclamp<16>(r) & ~1;
}

auto DSP::echo26() -> void {
  //left output volumes
  //(save sample for next clock so we can output both together)
  state._mainOut[0] = echoOutput(0);

  //echo feedback
  signed l = state._echoOut[0] + (int16)((state._echoIn[0] * (int8)REG(EFB)) >> 7);
  signed r = state._echoOut[1] + (int16)((state._echoIn[1] * (int8)REG(EFB)) >> 7);

  state._echoOut[0] = sclamp<16>(l) & ~1;
  state._echoOut[1] = sclamp<16>(r) & ~1;
}

auto DSP::echo27() -> void {
  //output
  signed outl = state._mainOut[0];
  signed outr = echoOutput(1);
  state._mainOut[0] = 0;
  state._mainOut[1] = 0;

  //TODO: global muting isn't this simple
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
