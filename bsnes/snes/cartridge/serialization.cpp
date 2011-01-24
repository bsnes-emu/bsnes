#ifdef CARTRIDGE_CPP

void Cartridge::serialize(serializer &s) {
  if(ram.data()) s.array(ram.data(), ram.size());
}

#endif
