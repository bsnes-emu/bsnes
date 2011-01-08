#ifdef ICD2_CPP

void ICD2::serialize(serializer &s) {
  Processor::serialize(s);

  GameBoy::system.runtosave();
  GameBoy::system.serialize_all(s);

  for(unsigned n = 0; n < 64; n++) s.array(packet[n].data);
  s.integer(packetsize);

  s.integer(joyp_id);
  s.integer(joyp15lock);
  s.integer(joyp14lock);
  s.integer(pulselock);
  s.integer(strobelock);
  s.integer(packetlock);
  s.array(joyp_packet.data);
  s.integer(packetoffset);
  s.integer(bitdata);
  s.integer(bitoffset);

  s.integer(r2181);
  s.integer(r2182);

  s.integer(r6000);
  s.integer(r6003);
  s.integer(r6004);
  s.integer(r6005);
  s.integer(r6006);
  s.integer(r6007);
  s.array(r7000);
  s.integer(r7800);
  s.integer(mlt_req);

  s.array(vram);
}

#endif
