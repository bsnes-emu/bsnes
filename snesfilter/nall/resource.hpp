#ifndef NALL_RESOURCE_HPP
#define NALL_RESOURCE_HPP

#include <nall/file.hpp>
#include <nall/zip.hpp>

namespace nall {

struct resource {
  //create resource with "zip -9 resource.zip resource"
  static bool encode(const char *outputFilename, const char *inputFilename) {
    file fp;
    if(fp.open(inputFilename, file::mode::read) == false) return false;
    unsigned size = fp.size();
    uint8_t *data = new uint8_t[size];
    fp.read(data, size);
    fp.close();

    fp.open(outputFilename, file::mode::write);
    fp.print("static const uint8_t data[", size, "] = {\n");
    uint8_t *p = data;
    while(size) {
      fp.print("  ");
      for(unsigned n = 0; n < 32 && size; n++, size--) {
        fp.print((unsigned)*p++, ",");
      }
      fp.print("\n");
    }
    fp.print("};\n");
    fp.close();

    delete[] data;
  }

  uint8_t *data;
  unsigned size;

  //extract first file from ZIP archive
  bool decode(const uint8_t *cdata, unsigned csize) {
    if(data) delete[] data;

    zip archive;
    if(archive.open(cdata, csize) == false) return false;
    if(archive.file.size() == 0) return false;
    bool result = archive.extract(archive.file[0], data, size);
    archive.close();

    return result;
  }

  resource() : data(0), size(0) {
  }

  ~resource() {
    if(data) delete[] data;
  }
};

}

#endif
