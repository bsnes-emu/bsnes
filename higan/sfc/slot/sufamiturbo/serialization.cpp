#ifdef SUFAMITURBO_CPP

void SufamiTurboCartridge::serialize(serializer& s) {
  s.array(ram.data(), ram.size());
}

#endif
