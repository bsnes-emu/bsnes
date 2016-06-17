auto ICD2::serialize(serializer& s) -> void {
  Thread::serialize(s);
  GameBoy::system.serializeAll(s);

  for(auto n : range(64)) s.array(packet[n].data);
  s.integer(packetSize);

  s.integer(joypID);
  s.integer(joyp15Lock);
  s.integer(joyp14Lock);
  s.integer(pulseLock);
  s.integer(strobeLock);
  s.integer(packetLock);
  s.array(joypPacket.data);
  s.integer(packetOffset);
  s.integer(bitData);
  s.integer(bitOffset);

  s.integer(r6003);
  s.integer(r6004);
  s.integer(r6005);
  s.integer(r6006);
  s.integer(r6007);
  s.array(r7000);
  s.integer(mltReq);

  s.array(output);
  s.integer(readBank);
  s.integer(readAddress);
  s.integer(writeBank);
  s.integer(writeAddress);
}
