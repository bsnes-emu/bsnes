auto ICD2::serialize(serializer& s) -> void {
  Thread::serialize(s);
  GameBoy::system.serializeAll(s);

  for(auto n : range(64)) s.array(packet[n].data);
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

  s.integer(r6003);
  s.integer(r6004);
  s.integer(r6005);
  s.integer(r6006);
  s.integer(r6007);
  s.array(r7000);
  s.integer(mlt_req);

  s.array(output);
  s.integer(read_bank);
  s.integer(read_addr);
  s.integer(write_bank);
  s.integer(write_addr);
}
