auto APU::serialize(serializer& s) -> void {
  Thread::serialize(s);

  filter.serialize(s);

  pulse[0].serialize(s);
  pulse[1].serialize(s);
  triangle.serialize(s);
  dmc.serialize(s);
  frame.serialize(s);

  s.integer(enabledChannels);
  s.integer(cartridgeSample);
}

auto APU::Filter::serialize(serializer& s) -> void {
  s.integer(hipassStrong);
  s.integer(hipassWeak);
  s.integer(lopass);
}

auto APU::Envelope::serialize(serializer& s) -> void {
  s.integer(speed);
  s.integer(useSpeedAsVolume);
  s.integer(loopMode);

  s.integer(reloadDecay);
  s.integer(decayCounter);
  s.integer(decayVolume);
}

auto APU::Sweep::serialize(serializer& s) -> void {
  s.integer(shift);
  s.integer(decrement);
  s.integer(period);
  s.integer(counter);
  s.integer(enable);
  s.integer(reload);
  s.integer(pulsePeriod);
}

auto APU::Pulse::serialize(serializer& s) -> void {
  s.integer(lengthCounter);

  envelope.serialize(s);
  sweep.serialize(s);

  s.integer(duty);
  s.integer(dutyCounter);

  s.integer(period);
  s.integer(periodCounter);
}

auto APU::Triangle::serialize(serializer& s) -> void {
  s.integer(lengthCounter);

  s.integer(linearLength);
  s.integer(haltLengthCounter);

  s.integer(period);
  s.integer(periodCounter);

  s.integer(stepCounter);
  s.integer(linearLengthCounter);
  s.integer(reloadLinear);
}

auto APU::Noise::serialize(serializer& s) -> void {
  s.integer(lengthCounter);

  envelope.serialize(s);

  s.integer(period);
  s.integer(periodCounter);

  s.integer(shortMode);
  s.integer(lfsr);
}

auto APU::DMC::serialize(serializer& s) -> void {
  s.integer(lengthCounter);
  s.integer(irqPending);

  s.integer(period);
  s.integer(periodCounter);

  s.integer(irqEnable);
  s.integer(loopMode);

  s.integer(dacLatch);
  s.integer(addrLatch);
  s.integer(lengthLatch);

  s.integer(readAddr);
  s.integer(dmaDelayCounter);

  s.integer(bitCounter);
  s.integer(dmaBufferValid);
  s.integer(dmaBuffer);

  s.integer(sampleValid);
  s.integer(sample);
}

auto APU::FrameCounter::serialize(serializer& s) -> void {
  s.integer(irqPending);

  s.integer(mode);
  s.integer(counter);
  s.integer(divider);
}
