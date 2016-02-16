#pragma once

#include <nall/beat/patch.hpp>
#include <nall/beat/linear.hpp>
#include <nall/beat/delta.hpp>

namespace nall {

struct bpsmulti {
  enum : uint {
    CreatePath = 0,
    CreateFile = 1,
    ModifyFile = 2,
    MirrorFile = 3,
  };

  enum : uint {
    OriginSource = 0,
    OriginTarget = 1,
  };

  auto create(const string& patchName, const string& sourcePath, const string& targetPath, bool delta = false, const string& metadata = "") -> bool {
    if(fp.open()) fp.close();
    fp.open(patchName, file::mode::write);
    checksum.reset();

    writeString("BPM1");  //signature
    writeNumber(metadata.length());
    writeString(metadata);

    lstring sourceList, targetList;
    ls(sourceList, sourcePath, sourcePath);
    ls(targetList, targetPath, targetPath);

    for(auto& targetName : targetList) {
      if(targetName.endsWith("/")) {
        targetName.rtrim("/");
        writeNumber(CreatePath | ((targetName.length() - 1) << 2));
        writeString(targetName);
      } else if(auto position = sourceList.find(targetName)) {  //if sourceName == targetName
        file sp, dp;
        sp.open({sourcePath, targetName}, file::mode::read);
        dp.open({targetPath, targetName}, file::mode::read);

        bool identical = sp.size() == dp.size();
        Hash::CRC32 cksum;

        for(uint n = 0; n < sp.size(); n++) {
          uint8_t byte = sp.read();
          if(identical && byte != dp.read()) identical = false;
          cksum.data(byte);
        }

        if(identical) {
          writeNumber(MirrorFile | ((targetName.length() - 1) << 2));
          writeString(targetName);
          writeNumber(OriginSource);
          writeChecksum(cksum.value());
        } else {
          writeNumber(ModifyFile | ((targetName.length() - 1) << 2));
          writeString(targetName);
          writeNumber(OriginSource);

          if(delta == false) {
            bpslinear patch;
            patch.source({sourcePath, targetName});
            patch.target({targetPath, targetName});
            patch.create({temppath(), "temp.bps"});
          } else {
            bpsdelta patch;
            patch.source({sourcePath, targetName});
            patch.target({targetPath, targetName});
            patch.create({temppath(), "temp.bps"});
          }

          auto buffer = file::read({temppath(), "temp.bps"});
          writeNumber(buffer.size());
          for(auto &byte : buffer) write(byte);
        }
      } else {
        writeNumber(CreateFile | ((targetName.length() - 1) << 2));
        writeString(targetName);
        auto buffer = file::read({targetPath, targetName});
        writeNumber(buffer.size());
        for(auto& byte : buffer) write(byte);
        writeChecksum(Hash::CRC32(buffer.data(), buffer.size()).value());
      }
    }

    //checksum
    writeChecksum(checksum.value());
    fp.close();
    return true;
  }

  auto apply(const string& patchName, const string& sourcePath, const string& targetPath) -> bool {
    directory::remove(targetPath);  //start with a clean directory
    directory::create(targetPath);

    if(fp.open()) fp.close();
    fp.open(patchName, file::mode::read);
    checksum.reset();

    if(readString(4) != "BPM1") return false;
    auto metadataLength = readNumber();
    while(metadataLength--) read();

    while(fp.offset() < fp.size() - 4) {
      auto encoding = readNumber();
      uint action = encoding & 3;
      uint targetLength = (encoding >> 2) + 1;
      string targetName = readString(targetLength);

      if(action == CreatePath) {
        directory::create({targetPath, targetName, "/"});
      } else if(action == CreateFile) {
        file fp;
        fp.open({targetPath, targetName}, file::mode::write);
        auto fileSize = readNumber();
        while(fileSize--) fp.write(read());
        uint32_t cksum = readChecksum();
      } else if(action == ModifyFile) {
        auto encoding = readNumber();
        string originPath = encoding & 1 ? targetPath : sourcePath;
        string sourceName = (encoding >> 1) == 0 ? targetName : readString(encoding >> 1);
        auto patchSize = readNumber();
        vector<uint8_t> buffer;
        buffer.resize(patchSize);
        for(uint n = 0; n < patchSize; n++) buffer[n] = read();
        bpspatch patch;
        patch.modify(buffer.data(), buffer.size());
        patch.source({originPath, sourceName});
        patch.target({targetPath, targetName});
        if(patch.apply() != bpspatch::result::success) return false;
      } else if(action == MirrorFile) {
        auto encoding = readNumber();
        string originPath = encoding & 1 ? targetPath : sourcePath;
        string sourceName = (encoding >> 1) == 0 ? targetName : readString(encoding >> 1);
        file::copy({originPath, sourceName}, {targetPath, targetName});
        uint32_t cksum = readChecksum();
      }
    }

    uint32_t cksum = checksum.value();
    if(read() != (uint8_t)(cksum >>  0)) return false;
    if(read() != (uint8_t)(cksum >>  8)) return false;
    if(read() != (uint8_t)(cksum >> 16)) return false;
    if(read() != (uint8_t)(cksum >> 24)) return false;

    fp.close();
    return true;
  }

protected:
  file fp;
  Hash::CRC32 checksum;

  //create() functions
  auto ls(lstring& list, const string& path, const string& basepath) -> void {
    lstring paths = directory::folders(path);
    for(auto& pathname : paths) {
      list.append(string{path, pathname}.ltrim(basepath, 1L));
      ls(list, {path, pathname}, basepath);
    }

    lstring files = directory::files(path);
    for(auto& filename : files) {
      list.append(string{path, filename}.ltrim(basepath, 1L));
    }
  }

  auto write(uint8_t data) -> void {
    fp.write(data);
    checksum.data(data);
  }

  auto writeNumber(uint64_t data) -> void {
    while(true) {
      uint64_t x = data & 0x7f;
      data >>= 7;
      if(data == 0) {
        write(0x80 | x);
        break;
      }
      write(x);
      data--;
    }
  }

  auto writeString(const string& text) -> void {
    uint length = text.length();
    for(uint n = 0; n < length; n++) write(text[n]);
  }

  auto writeChecksum(uint32_t cksum) -> void {
    write(cksum >>  0);
    write(cksum >>  8);
    write(cksum >> 16);
    write(cksum >> 24);
  }

  //apply() functions
  auto read() -> uint8_t {
    uint8_t data = fp.read();
    checksum.data(data);
    return data;
  }

  auto readNumber() -> uint64_t {
    uint64_t data = 0, shift = 1;
    while(true) {
      uint8_t x = read();
      data += (x & 0x7f) * shift;
      if(x & 0x80) break;
      shift <<= 7;
      data += shift;
    }
    return data;
  }

  auto readString(uint length) -> string {
    string text;
    text.resize(length + 1);
    char* p = text.get();
    while(length--) *p++ = read();
    return text;
  }

  auto readChecksum() -> uint32_t {
    uint32_t checksum = 0;
    checksum |= read() <<  0;
    checksum |= read() <<  8;
    checksum |= read() << 16;
    checksum |= read() << 24;
    return checksum;
  }
};

}
