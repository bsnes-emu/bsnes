#ifndef NALL_BEAT_ARCHIVE_HPP
#define NALL_BEAT_ARCHIVE_HPP

#include <nall/beat/base.hpp>

namespace nall {

struct beatArchive : beatBase {
  bool create(const string& beatname, string pathname, const string& metadata = "") {
    if(fp.open(beatname, file::mode::write) == false) return false;
    if(pathname.endswith("/") == false) pathname.append("/");

    checksum = ~0;
    writeString("BPA1");
    writeNumber(metadata.length());
    writeString(metadata);

    lstring list;
    ls(list, pathname, pathname);
    for(auto &name : list) {
      if(name.endswith("/")) {
        name.rtrim<1>("/");
        writeNumber(0 | ((name.length() - 1) << 1));
        writeString(name);
      } else {
        file stream;
        if(stream.open({pathname, name}, file::mode::read) == false) return false;
        writeNumber(1 | ((name.length() - 1) << 1));
        writeString(name);
        unsigned size = stream.size();
        writeNumber(size);
        uint32_t checksum = ~0;
        while(size--) {
          uint8_t data = stream.read();
          write(data);
          checksum = crc32_adjust(checksum, data);
        }
        writeChecksum(~checksum);
      }
    }

    writeChecksum(~checksum);
    fp.close();
    return true;
  }

  bool unpack(const string& beatname, string pathname) {
    if(fp.open(beatname, file::mode::read) == false) return false;
    if(pathname.endswith("/") == false) pathname.append("/");

    checksum = ~0;
    if(readString(4) != "BPA1") return false;
    unsigned length = readNumber();
    while(length--) read();

    directory::create(pathname);
    while(fp.offset() < fp.size() - 4) {
      unsigned data = readNumber();
      string name = readString((data >> 1) + 1);
      if(name.position("\\") || name.position("../")) return false;  //block path exploits

      if((data & 1) == 0) {
        directory::create({pathname, name});
      } else {
        file stream;
        if(stream.open({pathname, name}, file::mode::write) == false) return false;
        unsigned size = readNumber();
        uint32_t checksum = ~0;
        while(size--) {
          uint8_t data = read();
          stream.write(data);
          checksum = crc32_adjust(checksum, data);
        }
        if(readChecksum(~checksum) == false) return false;
      }
    }

    return readChecksum(~checksum);
  }
};

}

#endif
