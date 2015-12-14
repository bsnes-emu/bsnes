#ifndef NALL_STREAM_GZIP_HPP
#define NALL_STREAM_GZIP_HPP

#include <nall/decode/gzip.hpp>

namespace nall {

struct gzipstream : memorystream {
  using stream::read;
  using stream::write;

  gzipstream(const stream& stream) {
    uint size = stream.size();
    auto data = new uint8[size];
    stream.read(data, size);

    Decode::GZIP archive;
    bool result = archive.decompress(data, size);
    delete[] data;
    if(!result) return;

    psize = archive.size;
    pdata = new uint8_t[psize];
    memcpy(pdata, archive.data, psize);
  }

  ~gzipstream() {
    if(pdata) delete[] pdata;
  }
};

}

#endif
