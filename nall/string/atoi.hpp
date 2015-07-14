#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

auto string::integer() const -> intmax_t {
  if(beginsWith("0b") || beginsWith("0B")) return nall::binary(data());
  if(beginsWith("0o") || beginsWith("0O")) return nall::octal(data());
  if(beginsWith("0x") || beginsWith("0X")) return nall::hex(data());
  return nall::integer(data());
}

auto string::decimal() const -> uintmax_t {
  if(beginsWith("0b") || beginsWith("0B")) return nall::binary(data());
  if(beginsWith("0o") || beginsWith("0O")) return nall::octal(data());
  if(beginsWith("0x") || beginsWith("0X")) return nall::hex(data());
  return nall::decimal(data());
}

auto string::real() const -> double {
  return nall::real(data());
}

}

#endif
