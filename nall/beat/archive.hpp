#ifndef NALL_BEAT_ARCHIVE_HPP
#define NALL_BEAT_ARCHIVE_HPP

#include <nall/beat/base.hpp>

namespace nall {

struct beatArchive : beatBase {
  bool create(const string& beatname, string pathname, const string& metadata = "") {
    if(fp.open(beatname, file::mode::write) == false) return false;
    if(pathname.endsWith("/") == false) pathname.append("/");

    checksum.reset();
    writeString("BPA1");
    writeNumber(metadata.length());
    writeString(metadata);

    lstring list;
    ls(list, pathname, pathname);
    for(auto &name : list) {
      if(name.endsWith("/")) {
        name.rtrim("/");
        writeNumber(0 | ((name.length() - 1) << 1));
        writeString(name);
      } else {
        file stream;
        if(stream.open({pathname, name}, file::mode::read) == false) return false;
        writeNumber(1 | ((name.length() - 1) << 1));
        writeString(name);
        unsigned size = stream.size();
        writeNumber(size);
        Hash::CRC32 checksum;
        while(size--) {
          uint8_t data = stream.read();
          write(data);
          checksum.data(data);
        }
        writeChecksum(checksum.value());
      }
    }

    writeChecksum(checksum.value());
    fp.close();
    return true;
  }

  bool unpack(const string& beatname, string pathname) {
    if(fp.open(beatname, file::mode::read) == false) return false;
    if(pathname.endsWith("/") == false) pathname.append("/");

    checksum.reset();
    if(readString(4) != "BPA1") return false;
    unsigned length = readNumber();
    while(length--) read();

    directory::create(pathname);
    while(fp.offset() < fp.size() - 4) {
      unsigned data = readNumber();
      string name = readString((data >> 1) + 1);
      if(name.find("\\") || name.find("../")) return false;  //block path exploits

      if((data & 1) == 0) {
        directory::create({pathname, name});
      } else {
        file stream;
        if(stream.open({pathname, name}, file::mode::write) == false) return false;
        unsigned size = readNumber();
        Hash::CRC32 checksum;
        while(size--) {
          uint8_t data = read();
          stream.write(data);
          checksum.data(data);
        }
        if(readChecksum(checksum.value()) == false) return false;
      }
    }

    return readChecksum(checksum.value());
  }
};

}

#endif
