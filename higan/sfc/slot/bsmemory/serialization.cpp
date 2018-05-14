auto BSMemory::serialize(serializer& s) -> void {
  if(!readonly) s.array(memory.data(), memory.size());
}
