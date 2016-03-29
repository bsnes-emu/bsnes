inline auto DSP::voiceOutput(Voice& v, bool channel) -> void {
  //apply left/right volume
  int amp = (state._output * (int8)VREG(VOLL + channel)) >> 7;

  //add to output total
  state._mainOut[channel] += amp;
  state._mainOut[channel] = sclamp<16>(state._mainOut[channel]);

  //optionally add to echo total
  if(state._eon & v.vbit) {
    state._echoOut[channel] += amp;
    state._echoOut[channel] = sclamp<16>(state._echoOut[channel]);
  }
}

auto DSP::voice1(Voice& v) -> void {
  state._dirAddress = (state._dir << 8) + (state._srcn << 2);
  state._srcn = VREG(SRCN);
}

auto DSP::voice2(Voice& v) -> void {
  //read sample pointer (ignored if not needed)
  uint16 addr = state._dirAddress;
  if(!v.konDelay) addr += 2;
  uint8 lo = smp.apuram[(uint16)(addr + 0)];
  uint8 hi = smp.apuram[(uint16)(addr + 1)];
  state._brrNextAddress = ((hi << 8) + lo);

  state._adsr0 = VREG(ADSR0);

  //read pitch, spread over two clocks
  state._pitch = VREG(PITCHL);
}

auto DSP::voice3(Voice& v) -> void {
  voice3a(v);
  voice3b(v);
  voice3c(v);
}

auto DSP::voice3a(Voice& v) -> void {
  state._pitch += (VREG(PITCHH) & 0x3f) << 8;
}

auto DSP::voice3b(Voice& v) -> void {
  state._brrByte   = smp.apuram[(uint16)(v.brrAddress + v.brrOffset)];
  state._brrHeader = smp.apuram[(uint16)(v.brrAddress)];
}

auto DSP::voice3c(Voice& v) -> void {
  //pitch modulation using previous voice's output

  if(state._pmon & v.vbit) {
    state._pitch += ((state._output >> 5) * state._pitch) >> 10;
  }

  if(v.konDelay) {
    //get ready to start BRR decoding on next sample
    if(v.konDelay == 5) {
      v.brrAddress = state._brrNextAddress;
      v.brrOffset = 1;
      v.bufferOffset = 0;
      state._brrHeader = 0;  //header is ignored on this sample
    }

    //envelope is never run during KON
    v.envelope = 0;
    v.hiddenEnvelope = 0;

    //disable BRR decoding until last three samples
    v.gaussianOffset = 0;
    v.konDelay--;
    if(v.konDelay & 3) v.gaussianOffset = 0x4000;

    //pitch is never added during KON
    state._pitch = 0;
  }

  //gaussian interpolation
  int output = gaussianInterpolate(v);

  //noise
  if(state._non & v.vbit) {
    output = (int16)(state.noise << 1);
  }

  //apply envelope
  state._output = ((output * v.envelope) >> 11) & ~1;
  v._envxOut = v.envelope >> 4;

  //immediate silence due to end of sample or soft reset
  if(REG(FLG) & 0x80 || (state._brrHeader & 3) == 1) {
    v.envelopeMode = EnvelopeRelease;
    v.envelope = 0;
  }

  if(state.everyOtherSample) {
    //KOFF
    if(state._koff & v.vbit) {
      v.envelopeMode = EnvelopeRelease;
    }

    //KON
    if(state.kon & v.vbit) {
      v.konDelay = 5;
      v.envelopeMode = EnvelopeAttack;
    }
  }

  //run envelope for next sample
  if(!v.konDelay) envelopeRun(v);
}

auto DSP::voice4(Voice& v) -> void {
  //decode BRR
  state._looped = 0;
  if(v.gaussianOffset >= 0x4000) {
    brrDecode(v);
    v.brrOffset += 2;
    if(v.brrOffset >= 9) {
      //start decoding next BRR block
      v.brrAddress = (uint16)(v.brrAddress + 9);
      if(state._brrHeader & 1) {
        v.brrAddress = state._brrNextAddress;
        state._looped = v.vbit;
      }
      v.brrOffset = 1;
    }
  }

  //apply pitch
  v.gaussianOffset = (v.gaussianOffset & 0x3fff) + state._pitch;

  //keep from getting too far ahead (when using pitch modulation)
  if(v.gaussianOffset > 0x7fff) v.gaussianOffset = 0x7fff;

  //output left
  voiceOutput(v, 0);
}

auto DSP::voice5(Voice& v) -> void {
  //output right
  voiceOutput(v, 1);

  //ENDX, OUTX and ENVX won't update if you wrote to them 1-2 clocks earlier
  state.endxBuffer = REG(ENDX) | state._looped;

  //clear bit in ENDX if KON just began
  if(v.konDelay == 5) state.endxBuffer &= ~v.vbit;
}

auto DSP::voice6(Voice& v) -> void {
  state.outxBuffer = state._output >> 8;
}

auto DSP::voice7(Voice& v) -> void {
  //update ENDX
  REG(ENDX) = (uint8)state.endxBuffer;
  state.envxBuffer = v._envxOut;
}

auto DSP::voice8(Voice& v) -> void {
  //update OUTX
  VREG(OUTX) = (uint8)state.outxBuffer;
}

auto DSP::voice9(Voice& v) -> void {
  //update ENVX
  VREG(ENVX) = (uint8)state.envxBuffer;
}
