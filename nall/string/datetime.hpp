#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

auto string::date(time_t timestamp) -> string {
  if(timestamp == 0) timestamp = ::time(nullptr);
  tm* info = localtime(&timestamp);
  return {
    nall::decimal<4>(1900 + info->tm_year), "-",
    nall::decimal<2>(1 + info->tm_mon), "-",
    nall::decimal<2>(info->tm_mday)
  };
}

auto string::time(time_t timestamp) -> string {
  if(timestamp == 0) timestamp = ::time(nullptr);
  tm* info = localtime(&timestamp);
  return {
    nall::decimal<2>(info->tm_hour), ":",
    nall::decimal<2>(info->tm_min), ":",
    nall::decimal<2>(info->tm_sec)
  };
}

auto string::datetime(time_t timestamp) -> string {
  if(timestamp == 0) timestamp = ::time(nullptr);
  return {string::date(timestamp), " ", string::time(timestamp)};
}

}

#endif
