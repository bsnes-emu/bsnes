#pragma once

#include <nall/function.hpp>
#include <nall/string.hpp>

namespace nall::chrono {

//passage of time functions (from unknown epoch)

inline auto nanosecond() -> uint64_t {
  timespec tv;
  clock_gettime(CLOCK_MONOTONIC, &tv);
  return tv.tv_sec * 1'000'000'000 + tv.tv_nsec;
}

inline auto microsecond() -> uint64_t { return nanosecond() / 1'000; }
inline auto millisecond() -> uint64_t { return nanosecond() / 1'000'000; }
inline auto second() -> uint64_t { return nanosecond() / 1'000'000'000; }

inline auto benchmark(const function<void ()>& f, uint64_t times = 1) -> void {
  auto start = nanosecond();
  while(times--) f();
  auto end = nanosecond();
  print("[chrono::benchmark] ", (double)(end - start) / 1'000'000'000.0, "s\n");
}

//exact date/time functions (from system epoch)

struct timeinfo {
  timeinfo(
    uint year = 0, uint month = 0, uint day = 0,
    uint hour = 0, uint minute = 0, uint second = 0, uint weekday = 0
  ) : year(year), month(month), day(day),
      hour(hour), minute(minute), second(second), weekday(weekday) {
  }

  inline explicit operator bool() const { return month; }

  uint year;     //...
  uint month;    //1 - 12
  uint day;      //1 - 31
  uint hour;     //0 - 23
  uint minute;   //0 - 59
  uint second;   //0 - 60
  uint weekday;  //0 - 6
};

inline auto timestamp() -> uint64_t {
  return ::time(nullptr);
}

namespace utc {
  inline auto timeinfo(uint64_t time = 0) -> chrono::timeinfo {
    auto stamp = time ? (time_t)time : (time_t)timestamp();
    auto info = gmtime(&stamp);
    return {
      (uint)info->tm_year + 1900,
      (uint)info->tm_mon + 1,
      (uint)info->tm_mday,
      (uint)info->tm_hour,
      (uint)info->tm_min,
      (uint)info->tm_sec,
      (uint)info->tm_wday
    };
  }

  inline auto year(uint64_t timestamp = 0) -> string { return pad(timeinfo(timestamp).year, 4, '0'); }
  inline auto month(uint64_t timestamp = 0) -> string { return pad(timeinfo(timestamp).month, 2, '0'); }
  inline auto day(uint64_t timestamp = 0) -> string { return pad(timeinfo(timestamp).day, 2, '0'); }
  inline auto hour(uint64_t timestamp = 0) -> string { return pad(timeinfo(timestamp).hour, 2, '0'); }
  inline auto minute(uint64_t timestamp = 0) -> string { return pad(timeinfo(timestamp).minute, 2, '0'); }
  inline auto second(uint64_t timestamp = 0) -> string { return pad(timeinfo(timestamp).second, 2, '0'); }

  inline auto date(uint64_t timestamp = 0) -> string {
    auto t = timeinfo(timestamp);
    return {pad(t.year, 4, '0'), "-", pad(t.month, 2, '0'), "-", pad(t.day, 2, '0')};
  }

  inline auto time(uint64_t timestamp = 0) -> string {
    auto t = timeinfo(timestamp);
    return {pad(t.hour, 2, '0'), ":", pad(t.minute, 2, '0'), ":", pad(t.second, 2, '0')};
  }

  inline auto datetime(uint64_t timestamp = 0) -> string {
    auto t = timeinfo(timestamp);
    return {
      pad(t.year, 4, '0'), "-", pad(t.month, 2, '0'), "-", pad(t.day, 2, '0'), " ",
      pad(t.hour, 2, '0'), ":", pad(t.minute, 2, '0'), ":", pad(t.second, 2, '0')
    };
  }
}

namespace local {
  inline auto timeinfo(uint64_t time = 0) -> chrono::timeinfo {
    auto stamp = time ? (time_t)time : (time_t)timestamp();
    auto info = localtime(&stamp);
    return {
      (uint)info->tm_year + 1900,
      (uint)info->tm_mon + 1,
      (uint)info->tm_mday,
      (uint)info->tm_hour,
      (uint)info->tm_min,
      (uint)info->tm_sec,
      (uint)info->tm_wday
    };
  }

  inline auto year(uint64_t timestamp = 0) -> string { return pad(timeinfo(timestamp).year, 4, '0'); }
  inline auto month(uint64_t timestamp = 0) -> string { return pad(timeinfo(timestamp).month, 2, '0'); }
  inline auto day(uint64_t timestamp = 0) -> string { return pad(timeinfo(timestamp).day, 2, '0'); }
  inline auto hour(uint64_t timestamp = 0) -> string { return pad(timeinfo(timestamp).hour, 2, '0'); }
  inline auto minute(uint64_t timestamp = 0) -> string { return pad(timeinfo(timestamp).minute, 2, '0'); }
  inline auto second(uint64_t timestamp = 0) -> string { return pad(timeinfo(timestamp).second, 2, '0'); }

  inline auto date(uint64_t timestamp = 0) -> string {
    auto t = timeinfo(timestamp);
    return {pad(t.year, 4, '0'), "-", pad(t.month, 2, '0'), "-", pad(t.day, 2, '0')};
  }

  inline auto time(uint64_t timestamp = 0) -> string {
    auto t = timeinfo(timestamp);
    return {pad(t.hour, 2, '0'), ":", pad(t.minute, 2, '0'), ":", pad(t.second, 2, '0')};
  }

  inline auto datetime(uint64_t timestamp = 0) -> string {
    auto t = timeinfo(timestamp);
    return {
      pad(t.year, 4, '0'), "-", pad(t.month, 2, '0'), "-", pad(t.day, 2, '0'), " ",
      pad(t.hour, 2, '0'), ":", pad(t.minute, 2, '0'), ":", pad(t.second, 2, '0')
    };
  }
}

}
