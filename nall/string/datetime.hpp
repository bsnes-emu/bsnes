#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

string string::date() {
  time_t timestamp = ::time(nullptr);
  tm* info = localtime(&timestamp);
  return {
    format<4, '0'>(1900 + info->tm_year), "-",
    format<2, '0'>(1 + info->tm_mon), "-",
    format<2, '0'>(info->tm_mday)
  };
}

string string::time() {
  time_t timestamp = ::time(nullptr);
  tm* info = localtime(&timestamp);
  return {
    format<2, '0'>(info->tm_hour), ":",
    format<2, '0'>(info->tm_min), ":",
    format<2, '0'>(info->tm_sec)
  };
}

string string::datetime() {
  return {string::date(), " ", string::time()};
}

}

#endif
