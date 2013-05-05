#ifdef SHARPRTC_CPP

void SharpRTC::serialize(serializer& s) {
  Thread::serialize(s);

  s.integer((unsigned&)rtc_state);
  s.integer(rtc_index);

  s.integer(second);
  s.integer(minute);
  s.integer(hour);
  s.integer(day);
  s.integer(month);
  s.integer(year);
  s.integer(weekday);
}

#endif
