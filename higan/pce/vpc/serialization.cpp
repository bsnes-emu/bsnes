auto VPC::serialize(serializer& s) -> void {
  for(auto n : range(4)) {
    s.integer(settings[n].enableVDC0);
    s.integer(settings[n].enableVDC1);
    s.integer(settings[n].priority);
  }
  s.array(window);
  s.integer(select);
}
