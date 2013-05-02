#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

string string::date() {
  time_t timestamp = ::time(nullptr);
  tm* info = localtime(&timestamp);
  return {
    decimal<4, '0'>(1900 + info->tm_year), "-",
    decimal<2, '0'>(1 + info->tm_mon), "-",
    decimal<2, '0'>(info->tm_mday)
  };
}

string string::time() {
  time_t timestamp = ::time(nullptr);
  tm* info = localtime(&timestamp);
  return {
    decimal<2, '0'>(info->tm_hour), ":",
    decimal<2, '0'>(info->tm_min), ":",
    decimal<2, '0'>(info->tm_sec)
  };
}

string string::datetime() {
  return {string::date(), " ", string::time()};
}

}

#endif
