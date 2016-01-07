#pragma once

#include <nall/file.hpp>
#include <nall/filemap.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>

namespace nall {

struct bpsdelta {
  inline auto source(const uint8* data, uint size) -> void;
  inline auto target(const uint8* data, uint size) -> void;

  inline auto source(const string& filename) -> bool;
  inline auto target(const string& filename) -> bool;
  inline auto create(const string& filename, const string& metadata = "") -> bool;

protected:
  enum : uint { SourceRead, TargetRead, SourceCopy, TargetCopy };
  enum : uint { Granularity = 1 };

  struct Node {
    Node() = default;
    ~Node() { if(next) delete next; }
    uint offset = 0;
    Node* next = nullptr;
  };

  filemap sourceFile;
  const uint8* sourceData;
  uint sourceSize;

  filemap targetFile;
  const uint8* targetData;
  uint targetSize;
};

auto bpsdelta::source(const uint8* data, uint size) -> void {
  sourceData = data;
  sourceSize = size;
}

auto bpsdelta::target(const uint8* data, uint size) -> void {
  targetData = data;
  targetSize = size;
}

auto bpsdelta::source(const string& filename) -> bool {
  if(sourceFile.open(filename, filemap::mode::read) == false) return false;
  source(sourceFile.data(), sourceFile.size());
  return true;
}

auto bpsdelta::target(const string& filename) -> bool {
  if(targetFile.open(filename, filemap::mode::read) == false) return false;
  target(targetFile.data(), targetFile.size());
  return true;
}

auto bpsdelta::create(const string& filename, const string& metadata) -> bool {
  file modifyFile;
  if(modifyFile.open(filename, file::mode::write) == false) return false;

  Hash::CRC32 sourceChecksum, modifyChecksum;
  uint sourceRelativeOffset = 0, targetRelativeOffset = 0, outputOffset = 0;

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

  write('B');
  write('P');
  write('S');
  write('1');

  encode(sourceSize);
  encode(targetSize);

  uint markupSize = metadata.length();
  encode(markupSize);
  for(uint n = 0; n < markupSize; n++) write(metadata[n]);

  Node* sourceTree[65536];
  Node* targetTree[65536];
  for(uint n = 0; n < 65536; n++) sourceTree[n] = nullptr, targetTree[n] = nullptr;

  //source tree creation
  for(uint offset = 0; offset < sourceSize; offset++) {
    uint16 symbol = sourceData[offset + 0];
    sourceChecksum.data(symbol);
    if(offset < sourceSize - 1) symbol |= sourceData[offset + 1] << 8;
    Node *node = new Node;
    node->offset = offset;
    node->next = sourceTree[symbol];
    sourceTree[symbol] = node;
  }

  uint targetReadLength = 0;

  auto targetReadFlush = [&]() {
    if(targetReadLength) {
      encode(TargetRead | ((targetReadLength - 1) << 2));
      uint offset = outputOffset - targetReadLength;
      while(targetReadLength) write(targetData[offset++]), targetReadLength--;
    }
  };

  while(outputOffset < targetSize) {
    uint maxLength = 0, maxOffset = 0, mode = TargetRead;

    uint16 symbol = targetData[outputOffset + 0];
    if(outputOffset < targetSize - 1) symbol |= targetData[outputOffset + 1] << 8;

    { //source read
      uint length = 0, offset = outputOffset;
      while(offset < sourceSize && offset < targetSize && sourceData[offset] == targetData[offset]) {
        length++;
        offset++;
      }
      if(length > maxLength) maxLength = length, mode = SourceRead;
    }

    { //source copy
      Node* node = sourceTree[symbol];
      while(node) {
        uint length = 0, x = node->offset, y = outputOffset;
        while(x < sourceSize && y < targetSize && sourceData[x++] == targetData[y++]) length++;
        if(length > maxLength) maxLength = length, maxOffset = node->offset, mode = SourceCopy;
        node = node->next;
      }
    }

    { //target copy
      Node* node = targetTree[symbol];
      while(node) {
        uint length = 0, x = node->offset, y = outputOffset;
        while(y < targetSize && targetData[x++] == targetData[y++]) length++;
        if(length > maxLength) maxLength = length, maxOffset = node->offset, mode = TargetCopy;
        node = node->next;
      }

      //target tree append
      node = new Node;
      node->offset = outputOffset;
      node->next = targetTree[symbol];
      targetTree[symbol] = node;
    }

    { //target read
      if(maxLength < 4) {
        maxLength = min((uint)Granularity, targetSize - outputOffset);
        mode = TargetRead;
      }
    }

    if(mode != TargetRead) targetReadFlush();

    switch(mode) {
    case SourceRead:
      encode(SourceRead | ((maxLength - 1) << 2));
      break;
    case TargetRead:
      //delay write to group sequential TargetRead commands into one
      targetReadLength += maxLength;
      break;
    case SourceCopy:
    case TargetCopy:
      encode(mode | ((maxLength - 1) << 2));
      int relativeOffset;
      if(mode == SourceCopy) {
        relativeOffset = maxOffset - sourceRelativeOffset;
        sourceRelativeOffset = maxOffset + maxLength;
      } else {
        relativeOffset = maxOffset - targetRelativeOffset;
        targetRelativeOffset = maxOffset + maxLength;
      }
      encode((relativeOffset < 0) | (abs(relativeOffset) << 1));
      break;
    }

    outputOffset += maxLength;
  }

  targetReadFlush();

  for(uint n = 0; n < 32; n += 8) write(sourceChecksum.value() >> n);
  uint32 targetChecksum = Hash::CRC32(targetData, targetSize).value();
  for(uint n = 0; n < 32; n += 8) write(targetChecksum >> n);
  uint32 outputChecksum = modifyChecksum.value();
  for(uint n = 0; n < 32; n += 8) write(outputChecksum >> n);

  modifyFile.close();
  return true;
}

}
