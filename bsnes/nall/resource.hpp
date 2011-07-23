#ifndef NALL_RESOURCE_HPP
#define NALL_RESOURCE_HPP

#include <nall/file.hpp>
#include <nall/lzss.hpp>

namespace nall {

struct resource {
  static bool encode(const char *outputFilename, const char *inputFilename) {
    file fp;
    if(fp.open(inputFilename, file::mode::read) == false) return false;
    unsigned inputSize = fp.size();
    uint8_t *inputData = new uint8_t[inputSize];
    fp.read(inputData, inputSize);
    fp.close();

    unsigned outputSize;
    uint8_t *outputData;
    lzss::encode(outputData, outputSize, inputData, inputSize);

    fp.open(outputFilename, file::mode::write);
    fp.print("static const unsigned size = ", inputSize, ";\n");
    fp.print("static const uint8_t data[", outputSize, "] = {\n");
    uint8_t *p = outputData;
    while(outputSize) {
      fp.print("  ");
      for(unsigned n = 0; n < 32 && outputSize; n++, outputSize--) {
        fp.print((unsigned)*p++, ",");
      }
      fp.print("\n");
    }
    fp.print("};\n");
    fp.close();

    delete[] inputData;
    delete[] outputData;
  }

  uint8_t *data;
  unsigned size;

  bool decode(const uint8_t *inputData, unsigned outputSize) {
    if(data) delete[] data;
    lzss::decode(data, inputData, size = outputSize);
    return true;
  }

  resource() : data(0), size(0) {
  }

  ~resource() {
    if(data) delete[] data;
  }
};

}

#endif
