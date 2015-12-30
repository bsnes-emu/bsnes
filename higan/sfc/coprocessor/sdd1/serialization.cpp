auto SDD1::serialize(serializer& s) -> void {
  s.array(ram.data(), ram.size());

  s.integer(sdd1_enable);
  s.integer(xfer_enable);
  s.integer(dma_ready);
  s.array(mmc);

  for(auto n : range(8)) {
    s.integer(dma[n].addr);
    s.integer(dma[n].size);
  }
}
