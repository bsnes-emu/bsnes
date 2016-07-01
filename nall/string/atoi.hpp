#pragma once

namespace nall {

auto string::integer() const -> intmax_t {
  return toInteger(data());
}

auto string::natural() const -> uintmax_t {
  return toNatural(data());
}

auto string::hex() const -> uintmax_t {
  return toHex(data());
}

auto string::real() const -> double {
  return toReal(data());
}

}
