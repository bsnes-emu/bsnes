#ifndef NALL_BEAT_FILE_HPP
#define NALL_BEAT_FILE_HPP

namespace nall { namespace Beat {

struct File : file {
  using file::file;
  auto read() -> uint8_t override;
  auto write(uint8_t) -> void override;
  Hash::CRC32 checksum;
};

auto File::read() -> uint8_t {
  uint8_t data = file::read();
  checksum.data(data);
  return data;
}

auto File::write(uint8_t data) -> void {
  checksum.data(data);
  return file::write(data);
}

}}

#endif
