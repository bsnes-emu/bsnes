#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

namespace Hash {
  auto CRC16::digest() -> string {
    return hex<4>(value());
  }

  auto CRC32::digest() -> string {
    return hex<8>(value());
  }

  auto SHA256::digest() const -> string {
    string result;
    for(auto n : value()) result.append(hex<2>(n));
    return result;
  }
}

auto crc16(const string& self) -> string {
  return Hash::CRC16(self.data(), self.size()).digest();
}

auto crc32(const string& self) -> string {
  return Hash::CRC32(self.data(), self.size()).digest();
}

auto sha256(const string& self) -> string {
  return Hash::SHA256(self.data(), self.size()).digest();
}

}

#endif
