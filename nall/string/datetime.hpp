#pragma once

namespace nall {

auto string::date(time_t timestamp) -> string {
  if(timestamp == 0) timestamp = ::time(nullptr);
  tm* info = localtime(&timestamp);
  return {
    numeral(1900 + info->tm_year, 4L), "-",
    numeral(1 + info->tm_mon, 2L), "-",
    numeral(info->tm_mday, 2L)
  };
}

auto string::time(time_t timestamp) -> string {
  if(timestamp == 0) timestamp = ::time(nullptr);
  tm* info = localtime(&timestamp);
  return {
    numeral(info->tm_hour, 2L), ":",
    numeral(info->tm_min, 2L), ":",
    numeral(info->tm_sec, 2L)
  };
}

auto string::datetime(time_t timestamp) -> string {
  if(timestamp == 0) timestamp = ::time(nullptr);
  return {string::date(timestamp), " ", string::time(timestamp)};
}

}
