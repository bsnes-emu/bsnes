auto APU::Pulse::clockLength() -> void {
  if(envelope.loopMode == 0) {
    if(lengthCounter) lengthCounter--;
  }
}

auto APU::Pulse::clock() -> uint8 {
  if(!sweep.checkPeriod()) return 0;
  if(lengthCounter == 0) return 0;

  static const uint dutyTable[] = {1, 2, 4, 6};
  uint8 result = (dutyCounter < dutyTable[duty]) ? envelope.volume() : 0;
  if(sweep.pulsePeriod < 0x008) result = 0;

  if(--periodCounter == 0) {
    periodCounter = (sweep.pulsePeriod + 1) * 2;
    dutyCounter++;
  }

  return result;
}

auto APU::Pulse::power() -> void {
  envelope.power();
  sweep.power();

  lengthCounter = 0;

  duty = 0;
  dutyCounter = 0;
  period = 0;
  periodCounter = 1;
}
