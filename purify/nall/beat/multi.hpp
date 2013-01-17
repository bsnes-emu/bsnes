#ifndef NALL_BEAT_MULTI_HPP
#define NALL_BEAT_MULTI_HPP

#include <nall/beat/patch.hpp>
#include <nall/beat/linear.hpp>
#include <nall/beat/delta.hpp>

namespace nall {

struct bpsmulti {
  enum : unsigned {
    CreatePath = 0,
    CreateFile = 1,
    ModifyFile = 2,
    MirrorFile = 3,
  };

  enum : unsigned {
    OriginSource = 0,
    OriginTarget = 1,
  };

  bool create(const string &patchName, const string &sourcePath, const string &targetPath, bool delta = false, const string &metadata = "") {
    if(fp.open()) fp.close();
    fp.open(patchName, file::mode::write);
    checksum = ~0;

    writeString("BPM1");  //signature
    writeNumber(metadata.length());
    writeString(metadata);

    lstring sourceList, targetList;
    ls(sourceList, sourcePath, sourcePath);
    ls(targetList, targetPath, targetPath);

    for(auto &targetName : targetList) {
      if(targetName.endswith("/")) {
        targetName.rtrim<1>("/");
        writeNumber(CreatePath | ((targetName.length() - 1) << 2));
        writeString(targetName);
      } else if(auto position = sourceList.find(targetName)) {  //if sourceName == targetName
        file sp, dp;
        sp.open({sourcePath, targetName}, file::mode::read);
        dp.open({targetPath, targetName}, file::mode::read);

        bool identical = sp.size() == dp.size();
        uint32_t cksum = ~0;

        for(unsigned n = 0; n < sp.size(); n++) {
          uint8_t byte = sp.read();
          if(identical && byte != dp.read()) identical = false;
          cksum = crc32_adjust(cksum, byte);
        }

        if(identical) {
          writeNumber(MirrorFile | ((targetName.length() - 1) << 2));
          writeString(targetName);
          writeNumber(OriginSource);
          writeChecksum(~cksum);
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
        for(auto &byte : buffer) write(byte);
        writeChecksum(crc32_calculate(buffer.data(), buffer.size()));
      }
    }

    //checksum
    writeChecksum(~checksum);
    fp.close();
    return true;
  }

  bool apply(const string &patchName, const string &sourcePath, const string &targetPath) {
    directory::remove(targetPath);  //start with a clean directory
    directory::create(targetPath);

    if(fp.open()) fp.close();
    fp.open(patchName, file::mode::read);
    checksum = ~0;

    if(readString(4) != "BPM1") return false;
    auto metadataLength = readNumber();
    while(metadataLength--) read();

    while(fp.offset() < fp.size() - 4) {
      auto encoding = readNumber();
      unsigned action = encoding & 3;
      unsigned targetLength = (encoding >> 2) + 1;
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
        for(unsigned n = 0; n < patchSize; n++) buffer[n] = read();
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

    uint32_t cksum = ~checksum;
    if(read() != (uint8_t)(cksum >>  0)) return false;
    if(read() != (uint8_t)(cksum >>  8)) return false;
    if(read() != (uint8_t)(cksum >> 16)) return false;
    if(read() != (uint8_t)(cksum >> 24)) return false;

    fp.close();
    return true;
  }

protected:
  file fp;
  uint32_t checksum;

  //create() functions
  void ls(lstring &list, const string &path, const string &basepath) {
    lstring paths = directory::folders(path);
    for(auto &pathname : paths) {
      list.append(string{path, pathname}.ltrim<1>(basepath));
      ls(list, {path, pathname}, basepath);
    }

    lstring files = directory::files(path);
    for(auto &filename : files) {
      list.append(string{path, filename}.ltrim<1>(basepath));
    }
  }

  void write(uint8_t data) {
    fp.write(data);
    checksum = crc32_adjust(checksum, data);
  }

  void writeNumber(uint64_t data) {
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

  void writeString(const string &text) {
    unsigned length = text.length();
    for(unsigned n = 0; n < length; n++) write(text[n]);
  }

  void writeChecksum(uint32_t cksum) {
    write(cksum >>  0);
    write(cksum >>  8);
    write(cksum >> 16);
    write(cksum >> 24);
  }

  //apply() functions
  uint8_t read() {
    uint8_t data = fp.read();
    checksum = crc32_adjust(checksum, data);
    return data;
  }

  uint64_t readNumber() {
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

  string readString(unsigned length) {
    string text;
    text.reserve(length + 1);
    for(unsigned n = 0; n < length; n++) text[n] = read();
    text[length] = 0;
    return text;
  }

  uint32_t readChecksum() {
    uint32_t checksum = 0;
    checksum |= read() <<  0;
    checksum |= read() <<  8;
    checksum |= read() << 16;
    checksum |= read() << 24;
    return checksum;
  }
};

}

#endif
