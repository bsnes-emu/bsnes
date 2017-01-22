auto APU::Sweep::checkPeriod() -> bool {
  if(pulsePeriod > 0x7ff) return false;

  if(decrement == 0) {
    if((pulsePeriod + (pulsePeriod >> shift)) & 0x800) return false;
  }

  return true;
}

auto APU::Sweep::clock(uint channel) -> void {
  if(--counter == 0) {
    counter = period + 1;
    if(enable && shift && pulsePeriod > 8) {
      int delta = pulsePeriod >> shift;

      if(decrement) {
        pulsePeriod -= delta;
        if(channel == 0) pulsePeriod--;
      } else if((pulsePeriod + delta) < 0x800) {
        pulsePeriod += delta;
      }
    }
  }

  if(reload) {
    reload = false;
    counter = period + 1;
  }
}

auto APU::Sweep::power() -> void {
  shift = 0;
  decrement = 0;
  period = 0;
  counter = 1;
  enable = 0;
  reload = 0;
  pulsePeriod = 0;
}
