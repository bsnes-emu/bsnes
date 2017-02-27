auto YM2612::readStatus() -> uint8 {
  //d7 = busy
  return timerA.line << 0 | timerB.line << 1;
}

auto YM2612::writeAddress(uint9 data) -> void {
  io.address = data;
}

auto YM2612::writeData(uint8 data) -> void {
  switch(io.address) {

  //timer A period (high)
  case 0x024: {
    timerA.period.bits(2,9) = data.bits(0,7);
    break;
  }

  //timer A period (low)
  case 0x025: {
    timerA.period.bits(0,1) = data.bits(0,1);
    break;
  }

  //timer B period
  case 0x026: {
    timerB.period.bits(0,7) = data.bits(0,7);
    break;
  }

  //timer control
  case 0x027: {
    //d6,d7 = mode (unimplemented; treated as mode 0 always)

    //reload period on 0->1 transition
    if(!timerA.enable && data.bit(0)) timerA.counter = timerA.period;
    if(!timerB.enable && data.bit(1)) timerB.counter = timerB.period;

    timerA.enable = data.bit(0);
    timerB.enable = data.bit(1);
    timerA.irq = data.bit(2);
    timerB.irq = data.bit(3);

    if(data.bit(4)) timerA.line = 0;
    if(data.bit(5)) timerB.line = 0;

    break;
  }

  //DAC sample
  case 0x2a: {
    dac.sample = data;
    break;
  }

  //DAC enable
  case 0x2b: {
    dac.enable = data.bit(7);
    break;
  }

  }
}
