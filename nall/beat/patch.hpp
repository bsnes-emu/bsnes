#pragma once

#include <nall/file.hpp>
#include <nall/filemap.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>

namespace nall {

struct bpspatch {
  inline auto modify(const uint8* data, uint size) -> bool;
  inline auto source(const uint8* data, uint size) -> void;
  inline auto target(uint8* data, uint size) -> void;

  inline auto modify(const string& filename) -> bool;
  inline auto source(const string& filename) -> bool;
  inline auto target(const string& filename) -> bool;

  inline auto metadata() const -> string;
  inline auto size() const -> uint;

  enum result : uint {
    unknown,
    success,
    patch_too_small,
    patch_invalid_header,
    source_too_small,
    target_too_small,
    source_checksum_invalid,
    target_checksum_invalid,
    patch_checksum_invalid,
  };

  inline auto apply() -> result;

protected:
  enum : uint { SourceRead, TargetRead, SourceCopy, TargetCopy };

  filemap modifyFile;
  const uint8* modifyData;
  uint modifySize;

  filemap sourceFile;
  const uint8* sourceData;
  uint sourceSize;

  filemap targetFile;
  uint8* targetData;
  uint targetSize;

  uint modifySourceSize;
  uint modifyTargetSize;
  uint modifyMarkupSize;
  string metadataString;
};

auto bpspatch::modify(const uint8* data, uint size) -> bool {
  if(size < 19) return false;
  modifyData = data;
  modifySize = size;

  uint offset = 4;
  auto decode = [&]() -> uint64 {
    uint64 data = 0, shift = 1;
    while(true) {
      uint8 x = modifyData[offset++];
      data += (x & 0x7f) * shift;
      if(x & 0x80) break;
      shift <<= 7;
      data += shift;
    }
    return data;
  };

  modifySourceSize = decode();
  modifyTargetSize = decode();
  modifyMarkupSize = decode();

  char buffer[modifyMarkupSize + 1];
  for(uint n = 0; n < modifyMarkupSize; n++) buffer[n] = modifyData[offset++];
  buffer[modifyMarkupSize] = 0;
  metadataString = (const char*)buffer;

  return true;
}

auto bpspatch::source(const uint8* data, uint size) -> void {
  sourceData = data;
  sourceSize = size;
}

auto bpspatch::target(uint8* data, uint size) -> void {
  targetData = data;
  targetSize = size;
}

auto bpspatch::modify(const string& filename) -> bool {
  if(modifyFile.open(filename, filemap::mode::read) == false) return false;
  return modify(modifyFile.data(), modifyFile.size());
}

auto bpspatch::source(const string& filename) -> bool {
  if(sourceFile.open(filename, filemap::mode::read) == false) return false;
  source(sourceFile.data(), sourceFile.size());
  return true;
}

auto bpspatch::target(const string& filename) -> bool {
  file fp;
  if(fp.open(filename, file::mode::write) == false) return false;
  fp.truncate(modifyTargetSize);
  fp.close();

  if(targetFile.open(filename, filemap::mode::readwrite) == false) return false;
  target(targetFile.data(), targetFile.size());
  return true;
}

auto bpspatch::metadata() const -> string {
  return metadataString;
}

auto bpspatch::size() const -> uint {
  return modifyTargetSize;
}

auto bpspatch::apply() -> result {
  if(modifySize < 19) return result::patch_too_small;

  Hash::CRC32 modifyChecksum, targetChecksum;
  uint modifyOffset = 0, sourceRelativeOffset = 0, targetRelativeOffset = 0, outputOffset = 0;

  auto read = [&]() -> uint8 {
    uint8 data = modifyData[modifyOffset++];
    modifyChecksum.data(data);
    return data;
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

  auto write = [&](uint8 data) {
    targetData[outputOffset++] = data;
    targetChecksum.data(data);
  };

  if(read() != 'B') return result::patch_invalid_header;
  if(read() != 'P') return result::patch_invalid_header;
  if(read() != 'S') return result::patch_invalid_header;
  if(read() != '1') return result::patch_invalid_header;

  modifySourceSize = decode();
  modifyTargetSize = decode();
  modifyMarkupSize = decode();
  for(uint n = 0; n < modifyMarkupSize; n++) read();

  if(modifySourceSize > sourceSize) return result::source_too_small;
  if(modifyTargetSize > targetSize) return result::target_too_small;

  while(modifyOffset < modifySize - 12) {
    uint length = decode();
    uint mode = length & 3;
    length = (length >> 2) + 1;

    switch(mode) {
    case SourceRead:
      while(length--) write(sourceData[outputOffset]);
      break;
    case TargetRead:
      while(length--) write(read());
      break;
    case SourceCopy:
    case TargetCopy:
      int offset = decode();
      bool negative = offset & 1;
      offset >>= 1;
      if(negative) offset = -offset;

      if(mode == SourceCopy) {
        sourceRelativeOffset += offset;
        while(length--) write(sourceData[sourceRelativeOffset++]);
      } else {
        targetRelativeOffset += offset;
        while(length--) write(targetData[targetRelativeOffset++]);
      }
      break;
    }
  }

  uint32 modifySourceChecksum = 0, modifyTargetChecksum = 0, modifyModifyChecksum = 0;
  for(uint n = 0; n < 32; n += 8) modifySourceChecksum |= read() << n;
  for(uint n = 0; n < 32; n += 8) modifyTargetChecksum |= read() << n;
  uint32 checksum = modifyChecksum.value();
  for(uint n = 0; n < 32; n += 8) modifyModifyChecksum |= read() << n;

  uint32 sourceChecksum = Hash::CRC32(sourceData, modifySourceSize).value();

  if(sourceChecksum != modifySourceChecksum) return result::source_checksum_invalid;
  if(targetChecksum.value() != modifyTargetChecksum) return result::target_checksum_invalid;
  if(checksum != modifyModifyChecksum) return result::patch_checksum_invalid;

  return result::success;
}

}
