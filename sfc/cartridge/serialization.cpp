#ifdef CARTRIDGE_CPP

auto Cartridge::serialize(serializer& s) -> void {
  s.array(ram.data(), ram.size());
}

#endif
