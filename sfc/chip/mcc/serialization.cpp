#ifdef MCC_CPP

auto MCC::serialize(serializer& s) -> void {
  s.array(ram.data(), ram.size());
  s.array(psram.data(), psram.size());
}

#endif
