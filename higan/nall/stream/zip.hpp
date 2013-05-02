#ifndef NALL_STREAM_ZIP_HPP
#define NALL_STREAM_ZIP_HPP

#include <nall/unzip.hpp>

namespace nall {

struct zipstream : memorystream {
  using stream::read;
  using stream::write;

  zipstream(const stream& stream, const string& filter = "*") {
    unsigned size = stream.size();
    uint8_t* data = new uint8_t[size];
    stream.read(data, size);

    unzip archive;
    if(archive.open(data, size) == false) return;
    delete[] data;

    for(auto& file : archive.file) {
      if(file.name.wildcard(filter)) {
        auto buffer = archive.extract(file);
        psize = buffer.size();
        pdata = buffer.move();
        return;
      }
    }
  }

  ~zipstream() {
    if(pdata) delete[] pdata;
  }
};

}

#endif
