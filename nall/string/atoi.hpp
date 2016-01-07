#pragma once

namespace nall {

auto string::integer() const -> intmax {
  return nall::integer(data());
}

auto string::natural() const -> uintmax {
  return nall::natural(data());
}

auto string::real() const -> double {
  return nall::real(data());
}

}
