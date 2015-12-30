void DSP::serialize(serializer& s) {
  Thread::serialize(s);

  s.array(state.regs, 128);
  state.echoHistory[0].serialize(s);
  state.echoHistory[1].serialize(s);
  s.integer(state.echoHistoryOffset);

  s.integer(state.everyOtherSample);
  s.integer(state.kon);
  s.integer(state.noise);
  s.integer(state.counter);
  s.integer(state.echoOffset);
  s.integer(state.echoLength);

  s.integer(state.konBuffer);
  s.integer(state.endxBuffer);
  s.integer(state.envxBuffer);
  s.integer(state.outxBuffer);

  s.integer(state._pmon);
  s.integer(state._non);
  s.integer(state._eon);
  s.integer(state._dir);
  s.integer(state._koff);

  s.integer(state._brrNextAddress);
  s.integer(state._adsr0);
  s.integer(state._brrHeader);
  s.integer(state._brrByte);
  s.integer(state._srcn);
  s.integer(state._esa);
  s.integer(state._echoDisabled);

  s.integer(state._dirAddress);
  s.integer(state._pitch);
  s.integer(state._output);
  s.integer(state._looped);
  s.integer(state._echoPointer);

  s.array(state._mainOut, 2);
  s.array(state._echoOut, 2);
  s.array(state._echoIn, 2);

  for(auto n : range(8)) {
    voice[n].buffer.serialize(s);
    s.integer(voice[n].bufferOffset);
    s.integer(voice[n].gaussianOffset);
    s.integer(voice[n].brrAddress);
    s.integer(voice[n].brrOffset);
    s.integer(voice[n].vbit);
    s.integer(voice[n].vidx);
    s.integer(voice[n].konDelay);
    s.integer(voice[n].envelopeMode);
    s.integer(voice[n].envelope);
    s.integer(voice[n].hiddenEnvelope);
    s.integer(voice[n]._envxOut);
  }
}
