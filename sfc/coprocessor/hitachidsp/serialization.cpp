auto HitachiDSP::firmware() const -> vector<uint8> {
  vector<uint8> buffer;
  if(!cartridge.hasHitachiDSP()) return buffer;
  buffer.reserve(1024 * 3);
  for(auto n : range(1024)) {
    buffer.append(dataROM[n] >>  0);
    buffer.append(dataROM[n] >>  8);
    buffer.append(dataROM[n] >> 16);
  }
  return buffer;
}

auto HitachiDSP::serialize(serializer& s) -> void {
  HG51B::serialize(s);
  Thread::serialize(s);

  s.integer(mmio.dma);
  s.integer(mmio.dma_source);
  s.integer(mmio.dma_length);
  s.integer(mmio.dma_target);
  s.integer(mmio.r1f48);
  s.integer(mmio.program_offset);
  s.integer(mmio.r1f4c);
  s.integer(mmio.page_number);
  s.integer(mmio.program_counter);
  s.integer(mmio.r1f50);
  s.integer(mmio.r1f51);
  s.integer(mmio.r1f52);
  s.array(mmio.vector);
}
