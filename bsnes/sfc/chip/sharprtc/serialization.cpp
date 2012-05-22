#ifdef SHARPRTC_CPP

void SharpRTC::serialize(serializer &s) {
  s.array(rtc);
  s.integer(rtc_mode);
  s.integer(rtc_index);
}

#endif
