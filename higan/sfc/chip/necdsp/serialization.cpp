#ifdef NECDSP_CPP

vector<uint8> NECDSP::firmware() {
  vector<uint8> buffer;
  if(cartridge.has_necdsp() == false) return buffer;
  unsigned plength = 2048, dlength = 1024;
  if(revision == Revision::uPD96050) plength = 16384, dlength = 2048;
  buffer.reserve(plength * 3 + dlength * 2);

  for(unsigned n = 0; n < plength; n++) {
    buffer.append(programROM[n] >>  0);
    buffer.append(programROM[n] >>  8);
    buffer.append(programROM[n] >> 16);
  }

  for(unsigned n = 0; n < dlength; n++) {
    buffer.append(dataROM[n] >> 0);
    buffer.append(dataROM[n] >> 8);
  }

  return buffer;
}

void NECDSP::serialize(serializer &s) {
  uPD96050::serialize(s);
  Thread::serialize(s);
}

#endif
