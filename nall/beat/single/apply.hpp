#pragma once

namespace nall { namespace Beat { namespace Single {

inline auto apply(
  const uint8_t* sourceData, uint64_t sourceSize,
  const uint8_t* beatData, uint64_t beatSize,
  maybe<string&> manifest = {}, maybe<string&> result = {}
) -> maybe<vector<uint8_t>> {
  #define error(text) { if(result) *result = {"error: ", text}; return {}; }
  #define warning(text) { if(result) *result = {"warning: ", text}; return targetData; }
  #define success() { if(result) *result = ""; return targetData; }
  if(beatSize < 19) error("beat size mismatch");

  vector<uint8_t> targetData;

  uint beatOffset = 0;
  auto read = [&]() -> uint8_t {
    return beatData[beatOffset++];
  };
  auto decode = [&]() -> uint64_t {
    uint64_t data = 0, shift = 1;
    while(true) {
      uint8_t x = read();
      data += (x & 0x7f) * shift;
      if(x & 0x80) break;
      shift <<= 7;
      data += shift;
    }
    return data;
  };

  auto targetOffset = 0;
  auto write = [&](uint8_t data) {
    targetData.append(data);
  };

  if(read() != 'B') error("beat header invalid");
  if(read() != 'P') error("beat header invalid");
  if(read() != 'S') error("beat header invalid");
  if(read() != '1') error("beat version mismatch");
  if(decode() != sourceSize) error("source size mismatch");
  uint targetSize = decode();
  targetData.reserve(targetSize);
  auto metadataSize = decode();
  for(uint n : range(metadataSize)) {
    auto data = read();
    if(manifest) manifest->append((char)data);
  }

  enum : uint { SourceRead, TargetRead, SourceCopy, TargetCopy };

  uint sourceRelativeOffset = 0, targetRelativeOffset = 0;
  while(beatOffset < beatSize - 12) {
    uint length = decode();
    uint mode = length & 3;
    length = (length >> 2) + 1;

    if(mode == SourceRead) {
      while(length--) write(sourceData[targetOffset]);
    } else if(mode == TargetRead) {
      while(length--) write(read());
    } else {
      int offset = decode();
      offset = offset & 1 ? -(offset >> 1) : (offset >> 1);
      if(mode == SourceCopy) {
        sourceRelativeOffset += offset;
        while(length--) write(sourceData[sourceRelativeOffset++]);
      } else {
        targetRelativeOffset += offset;
        while(length--) write(targetData[targetRelativeOffset++]);
      }
    }
  }

  uint32_t sourceHash = 0, targetHash = 0, beatHash = 0;
  for(uint shift : range(0, 32, 8)) sourceHash |= read() << shift;
  for(uint shift : range(0, 32, 8)) targetHash |= read() << shift;
  for(uint shift : range(0, 32, 8)) beatHash |= read() << shift;

  if(targetOffset != targetSize) warning("target size mismatch");
  if(sourceHash != Hash::CRC32(sourceData, sourceSize).value()) warning("source hash mismatch");
  if(targetHash != Hash::CRC32(targetData).value()) warning("target hash mismatch");
  if(beatHash != Hash::CRC32(beatData, beatSize - 4).value()) warning("beat hash mismatch");

  success();
  #undef error
  #undef warning
  #undef success
}

}}}
