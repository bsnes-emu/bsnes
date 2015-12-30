auto Input::serialize(serializer& s) -> void {
  s.integer((uint&)port1);
  s.integer((uint&)port2);

  s.integer(latchdata);
  s.integer(counter1);
  s.integer(counter2);
}
