#ifndef NALL_ATOI_HPP
#define NALL_ATOI_HPP

#include <nall/stdint.hpp>
#include <nall/varint.hpp>

namespace nall {

constexpr inline auto binary_(const char* s, uintmax sum = 0) -> uintmax {
  return (
    *s == '0' || *s == '1' ? binary_(s + 1, (sum << 1) | *s - '0') :
    *s == '\'' ? binary_(s + 1, sum) :
    sum
  );
}

constexpr inline auto octal_(const char* s, uintmax sum = 0) -> uintmax {
  return (
    *s >= '0' && *s <= '7' ? octal_(s + 1, (sum << 3) | *s - '0') :
    *s == '\'' ? octal_(s + 1, sum) :
    sum
  );
}

constexpr inline auto natural_(const char* s, uintmax sum = 0) -> uintmax {
  return (
    *s >= '0' && *s <= '9' ? natural_(s + 1, (sum * 10) + *s - '0') :
    *s == '\'' ? natural_(s + 1, sum) :
    sum
  );
}

constexpr inline auto hex_(const char* s, uintmax sum = 0) -> uintmax {
  return (
    *s >= 'A' && *s <= 'F' ? hex_(s + 1, (sum << 4) | *s - 'A' + 10) :
    *s >= 'a' && *s <= 'f' ? hex_(s + 1, (sum << 4) | *s - 'a' + 10) :
    *s >= '0' && *s <= '9' ? hex_(s + 1, (sum << 4) | *s - '0') :
    *s == '\'' ? hex_(s + 1, sum) :
    sum
  );
}

//

constexpr inline auto binary(const char* s) -> uintmax {
  return (
    *s == '0' && *(s + 1) == 'B' ? binary_(s + 2) :
    *s == '0' && *(s + 1) == 'b' ? binary_(s + 2) :
    *s == '%' ? binary_(s + 1) :
    binary_(s)
  );
}

constexpr inline auto octal(const char* s) -> uintmax {
  return (
    *s == '0' && *(s + 1) == 'O' ? octal_(s + 2) :
    *s == '0' && *(s + 1) == 'o' ? octal_(s + 2) :
    octal_(s)
  );
}

constexpr inline auto integer(const char* s) -> intmax {
  return (
    *s == '+' ? +natural_(s + 1) :
    *s == '-' ? -natural_(s + 1) :
    natural_(s)
  );
}

constexpr inline auto natural(const char* s) -> uintmax {
  return (
    natural_(s)
  );
}

constexpr inline auto hex(const char* s) -> uintmax {
  return (
    *s == '0' && *(s + 1) == 'X' ? hex_(s + 2) :
    *s == '0' && *(s + 1) == 'x' ? hex_(s + 2) :
    *s == '$' ? hex_(s + 1) :
    hex_(s)
  );
}

inline auto real(const char* s) -> double {
  return atof(s);
}

}

#endif
