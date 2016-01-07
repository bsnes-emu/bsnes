#pragma once

#include <nall/file.hpp>
#include <nall/filemap.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>

namespace nall {

struct bpsmetadata {
  inline auto load(const string& filename) -> bool;
  inline auto save(const string& filename, const string& metadata) -> bool;
  inline auto metadata() const -> string;

protected:
  file sourceFile;
  string metadataString;
};

auto bpsmetadata::load(const string& filename) -> bool {
  if(sourceFile.open(filename, file::mode::read) == false) return false;

  auto read = [&]() -> uint8_t {
    return sourceFile.read();
  };

  auto decode = [&]() -> uint64 {
    uint64 data = 0, shift = 1;
    while(true) {
      uint8 x = read();
      data += (x & 0x7f) * shift;
      if(x & 0x80) break;
      shift <<= 7;
      data += shift;
    }
    return data;
  };

  if(read() != 'B') return false;
  if(read() != 'P') return false;
  if(read() != 'S') return false;
  if(read() != '1') return false;
  decode();
  decode();
  uint metadataSize = decode();
  char data[metadataSize + 1];
  for(uint n = 0; n < metadataSize; n++) data[n] = read();
  data[metadataSize] = 0;
  metadataString = (const char*)data;

  return true;
}

auto bpsmetadata::save(const string& filename, const string& metadata) -> bool {
  file targetFile;
  if(targetFile.open(filename, file::mode::write) == false) return false;
  if(sourceFile.open() == false) return false;
  sourceFile.seek(0);

  auto read = [&]() -> uint8 {
    return sourceFile.read();
  };

  auto decode = [&]() -> uint64 {
    uint64 data = 0, shift = 1;
    while(true) {
      uint8 x = read();
      data += (x & 0x7f) * shift;
      if(x & 0x80) break;
      shift <<= 7;
      data += shift;
    }
    return data;
  };

  Hash::CRC32 checksum;

  auto write = [&](uint8 data) {
    targetFile.write(data);
    checksum.data(data);
  };

  auto encode = [&](uint64 data) {
    while(true) {
      uint64 x = data & 0x7f;
      data >>= 7;
      if(data == 0) {
        write(0x80 | x);
        break;
      }
      write(x);
      data--;
    }
  };

  for(uint n = 0; n < 4; n++) write(read());
  encode(decode());
  encode(decode());
  uint sourceLength = decode();
  uint targetLength = metadata.length();
  encode(targetLength);
  sourceFile.seek(sourceLength, file::index::relative);
  for(uint n = 0; n < targetLength; n++) write(metadata[n]);
  uint length = sourceFile.size() - sourceFile.offset() - 4;
  for(uint n = 0; n < length; n++) write(read());
  uint32 outputChecksum = checksum.value();
  for(uint n = 0; n < 32; n += 8) write(outputChecksum >> n);

  targetFile.close();
  return true;
}

auto bpsmetadata::metadata() const -> string {
  return metadataString;
}

}
