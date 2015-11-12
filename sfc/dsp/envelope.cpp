auto DSP::envelopeRun(Voice& v) -> void {
  signed envelope = v.envelope;

  if(v.envelopeMode == EnvelopeRelease) {  //60%
    envelope -= 0x8;
    if(envelope < 0) envelope = 0;
    v.envelope = envelope;
    return;
  }

  signed rate;
  signed envelopeData = VREG(ADSR1);
  if(state._adsr0 & 0x80) {  //99% ADSR
    if(v.envelopeMode >= EnvelopeDecay) {  //99%
      envelope--;
      envelope -= envelope >> 8;
      rate = envelopeData & 0x1f;
      if(v.envelopeMode == EnvelopeDecay) {  //1%
        rate = ((state._adsr0 >> 3) & 0x0e) + 0x10;
      }
    } else {  //env_attack
      rate = ((state._adsr0 & 0x0f) << 1) + 1;
      envelope += rate < 31 ? 0x20 : 0x400;
    }
  } else {  //GAIN
    envelopeData = VREG(GAIN);
    signed mode = envelopeData >> 5;
    if(mode < 4) {  //direct
      envelope = envelopeData << 4;
      rate = 31;
    } else {
      rate = envelopeData & 0x1f;
      if(mode == 4) {  //4: linear decrease
        envelope -= 0x20;
      } else if(mode < 6) {  //5: exponential decrease
        envelope--;
        envelope -= envelope >> 8;
      } else {  //6, 7: linear increase
        envelope += 0x20;
        if(mode > 6 && (unsigned)v.hiddenEnvelope >= 0x600) {
          envelope += 0x8 - 0x20;  //7: two-slope linear increase
        }
      }
    }
  }

  //sustain level
  if((envelope >> 8) == (envelopeData >> 5) && v.envelopeMode == EnvelopeDecay) v.envelopeMode = EnvelopeSustain;
  v.hiddenEnvelope = envelope;

  //unsigned cast because linear decrease underflowing also triggers this
  if((unsigned)envelope > 0x7ff) {
    envelope = (envelope < 0 ? 0 : 0x7ff);
    if(v.envelopeMode == EnvelopeAttack) v.envelopeMode = EnvelopeDecay;
  }

  if(counterPoll(rate)) v.envelope = envelope;
}
