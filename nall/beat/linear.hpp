#ifndef NALL_BEAT_LINEAR_HPP
#define NALL_BEAT_LINEAR_HPP

#include <nall/file.hpp>
#include <nall/filemap.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>

namespace nall {

struct bpslinear {
  inline auto source(const uint8* data, uint size) -> void;
  inline auto target(const uint8* data, uint size) -> void;

  inline auto source(const string& filename) -> bool;
  inline auto target(const string& filename) -> bool;
  inline auto create(const string& filename, const string& metadata = "") -> bool;

protected:
  enum : uint { SourceRead, TargetRead, SourceCopy, TargetCopy };
  enum : uint { Granularity = 1 };

  filemap sourceFile;
  const uint8* sourceData;
  uint sourceSize;

  filemap targetFile;
  const uint8* targetData;
  uint targetSize;
};

auto bpslinear::source(const uint8* data, uint size) -> void {
  sourceData = data;
  sourceSize = size;
}

auto bpslinear::target(const uint8* data, uint size) -> void {
  targetData = data;
  targetSize = size;
}

auto bpslinear::source(const string& filename) -> bool {
  if(sourceFile.open(filename, filemap::mode::read) == false) return false;
  source(sourceFile.data(), sourceFile.size());
  return true;
}

auto bpslinear::target(const string& filename) -> bool {
  if(targetFile.open(filename, filemap::mode::read) == false) return false;
  target(targetFile.data(), targetFile.size());
  return true;
}

auto bpslinear::create(const string& filename, const string& metadata) -> bool {
  file modifyFile;
  if(modifyFile.open(filename, file::mode::write) == false) return false;

  Hash::CRC32 modifyChecksum;
  uint targetRelativeOffset = 0, outputOffset = 0;

  auto write = [&](uint8 data) {
    modifyFile.write(data);
    modifyChecksum.data(data);
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

  uint targetReadLength = 0;

  auto targetReadFlush = [&]() {
    if(targetReadLength) {
      encode(TargetRead | ((targetReadLength - 1) << 2));
      uint offset = outputOffset - targetReadLength;
      while(targetReadLength) write(targetData[offset++]), targetReadLength--;
    }
  };

  write('B');
  write('P');
  write('S');
  write('1');

  encode(sourceSize);
  encode(targetSize);

  uint markupSize = metadata.length();
  encode(markupSize);
  for(uint n = 0; n < markupSize; n++) write(metadata[n]);

  while(outputOffset < targetSize) {
    uint sourceLength = 0;
    for(uint n = 0; outputOffset + n < min(sourceSize, targetSize); n++) {
      if(sourceData[outputOffset + n] != targetData[outputOffset + n]) break;
      sourceLength++;
    }

    uint rleLength = 0;
    for(uint n = 1; outputOffset + n < targetSize; n++) {
      if(targetData[outputOffset] != targetData[outputOffset + n]) break;
      rleLength++;
    }

    if(rleLength >= 4) {
      //write byte to repeat
      targetReadLength++;
      outputOffset++;
      targetReadFlush();

      //copy starting from repetition byte
      encode(TargetCopy | ((rleLength - 1) << 2));
      uint relativeOffset = (outputOffset - 1) - targetRelativeOffset;
      encode(relativeOffset << 1);
      outputOffset += rleLength;
      targetRelativeOffset = outputOffset - 1;
    } else if(sourceLength >= 4) {
      targetReadFlush();
      encode(SourceRead | ((sourceLength - 1) << 2));
      outputOffset += sourceLength;
    } else {
      targetReadLength += Granularity;
      outputOffset += Granularity;
    }
  }

  targetReadFlush();

  uint32 sourceChecksum = Hash::CRC32(sourceData, sourceSize).value();
  for(uint n = 0; n < 32; n += 8) write(sourceChecksum >> n);
  uint32 targetChecksum = Hash::CRC32(targetData, targetSize).value();
  for(uint n = 0; n < 32; n += 8) write(targetChecksum >> n);
  uint32 outputChecksum = modifyChecksum.value();
  for(uint n = 0; n < 32; n += 8) write(outputChecksum >> n);

  modifyFile.close();
  return true;
}

}

#endif
