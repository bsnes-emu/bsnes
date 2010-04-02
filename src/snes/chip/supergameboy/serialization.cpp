#ifdef SUPERGAMEBOY_CPP

void SuperGameBoy::serialize(serializer &s) {
  s.integer(row);
  if(sgb_serialize) sgb_serialize(s);
}

#endif
