#pragma once

#include <nall/decode/zip.hpp>

namespace nall {

struct zipstream : memorystream {
  using stream::read;
  using stream::write;

  zipstream(const stream& stream, const string& filter = "*") {
    uint size = stream.size();
    auto data = new uint8_t[size];
    stream.read(data, size);

    Decode::ZIP archive;
    if(archive.open(data, size) == false) return;
    delete[] data;

    for(auto& file : archive.file) {
      if(file.name.match(filter)) {
        auto buffer = archive.extract(file);
        psize = buffer.size();
        pdata = buffer.release();
        return;
      }
    }
  }

  ~zipstream() {
    if(pdata) delete[] pdata;
  }
};

}
