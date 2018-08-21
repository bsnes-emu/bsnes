#pragma once

#include <nall/encode/dictionary.hpp>

namespace nall { namespace Beat { namespace Single {

inline auto create(
  const uint8_t* sourceData, uint64_t sourceSize,
  const uint8_t* targetData, uint64_t targetSize,
  const string& manifest = {}
) -> maybe<vector<uint8_t>> {
  vector<uint8_t> beatData;

  auto write = [&](uint8_t data) {
    beatData.append(data);
  };

  auto encode = [&](uint64_t data) {
    while(true) {
      uint64_t x = data & 0x7f;
      data >>= 7;
      if(data == 0) { write(0x80 | x); break; }
      write(x);
      data--;
    }
  };

  write('B'), write('P'), write('S'), write('1');
  encode(sourceSize), encode(targetSize), encode(manifest.size());
  for(auto& byte : manifest) write(byte);

  auto read = [&](const uint8_t* data, uint size, uint offset) -> uint {
    if(offset + 3 >= size) return 0;
    return data[offset + 0] << 24 | data[offset + 1] << 16 | data[offset + 2] << 8 | data[offset + 3] << 0;
  };

  Encode::Dictionary sourceDictionary(sourceData, sourceSize);
  Encode::Dictionary targetDictionary(targetData, targetSize);
  sourceDictionary.scan();
  targetDictionary.scan();

  enum : uint { SourceRead, TargetRead, SourceCopy, TargetCopy };
  uint outputOffset = 0, sourceRelativeOffset = 0, targetRelativeOffset = 0;

  uint targetReadLength = 0;
  auto flush = [&] {
    if(!targetReadLength) return;
    encode(TargetRead | ((targetReadLength - 1) << 2));
    uint offset = outputOffset - targetReadLength;
    while(targetReadLength) write(targetData[offset++]), targetReadLength--;
  };

  uint longestSize = max(sourceSize, targetSize);
  while(outputOffset < targetSize) {
    uint mode = TargetRead, longestLength = 3, longestOffset = 0;
    uint prefix = read(targetData, targetSize, outputOffset), lower, upper;

    uint length = 0, offset = outputOffset;
    while(offset < longestOffset) {
      if(sourceData[offset] != targetData[offset]) break;
      length++, offset++;
    }
    if(length > longestLength) {
      mode = SourceRead, longestLength = length;
    }

    sourceDictionary.find(prefix, lower, upper);
    for(uint index = lower; index <= upper; index++) {
      uint length = 0, sourceOffset = sourceDictionary[index], targetOffset = outputOffset;
      while(sourceOffset < sourceSize && targetOffset < targetSize) {
        if(sourceData[sourceOffset++] != targetData[targetOffset++]) break;
        length++;
      }
      if(length > longestLength) {
        mode = SourceCopy, longestLength = length, longestOffset = sourceDictionary[index];
      }
    }

    targetDictionary.find(prefix, lower, upper);
    for(uint index = lower; index <= upper; index++) {
      uint length = 0, sourceOffset = targetDictionary[index], targetOffset = outputOffset;
      if(sourceOffset >= outputOffset) continue;
      while(targetOffset < targetSize) {
        if(targetData[sourceOffset++] != targetData[targetOffset++]) break;
        length++;
      }
      if(length > longestLength) {
        mode = TargetCopy, longestLength = length, longestOffset = targetDictionary[index];
      }
    }

    if(mode == TargetRead) {
      targetReadLength++;  //queue writes to group sequential commands
      outputOffset++;
    } else {
      flush();
      encode(mode | ((longestLength - 1) << 2));
      if(mode == SourceCopy) {
        int relativeOffset = longestOffset - sourceRelativeOffset;
        sourceRelativeOffset = longestOffset + longestLength;
        encode(relativeOffset < 0 | abs(relativeOffset) << 1);
      }
      if(mode == TargetCopy) {
        int relativeOffset = longestOffset - targetRelativeOffset;
        targetRelativeOffset = longestOffset + longestLength;
        encode(relativeOffset < 0 | abs(relativeOffset) << 1);
      }
      outputOffset += longestLength;
    }
  }
  flush();

  auto sourceHash = Hash::CRC32(sourceData, sourceSize);
  for(uint shift : range(0, 32, 8)) write(sourceHash.value() >> shift);
  auto targetHash = Hash::CRC32(targetData, targetSize);
  for(uint shift : range(0, 32, 8)) write(targetHash.value() >> shift);
  auto beatHash = Hash::CRC32(beatData);
  for(uint shift : range(0, 32, 8)) write(beatHash.value() >> shift);

  return beatData;
}

}}}
