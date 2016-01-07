#pragma once

#include <nall/filemap.hpp>
#include <nall/string.hpp>
#include <nall/vector.hpp>
#include <nall/decode/inflate.hpp>

namespace nall { namespace Decode {

struct ZIP {
  struct File {
    string name;
    const uint8* data;
    uint size;
    uint csize;
    uint cmode;  //0 = uncompressed, 8 = deflate
    uint crc32;
  };

  ~ZIP() {
    close();
  }

  auto open(const string& filename) -> bool {
    close();
    if(fm.open(filename, filemap::mode::read) == false) return false;
    if(open(fm.data(), fm.size()) == false) {
      fm.close();
      return false;
    }
    return true;
  }

  auto open(const uint8* data, uint size) -> bool {
    if(size < 22) return false;

    filedata = data;
    filesize = size;

    file.reset();

    const uint8* footer = data + size - 22;
    while(true) {
      if(footer <= data + 22) return false;
      if(read(footer, 4) == 0x06054b50) {
        uint commentlength = read(footer + 20, 2);
        if(footer + 22 + commentlength == data + size) break;
      }
      footer--;
    }
    const uint8* directory = data + read(footer + 16, 4);

    while(true) {
      uint signature = read(directory + 0, 4);
      if(signature != 0x02014b50) break;

      File file;
      file.cmode = read(directory + 10, 2);
      file.crc32 = read(directory + 16, 4);
      file.csize = read(directory + 20, 4);
      file.size  = read(directory + 24, 4);

      uint namelength = read(directory + 28, 2);
      uint extralength = read(directory + 30, 2);
      uint commentlength = read(directory + 32, 2);

      char* filename = new char[namelength + 1];
      memcpy(filename, directory + 46, namelength);
      filename[namelength] = 0;
      file.name = filename;
      delete[] filename;

      uint offset = read(directory + 42, 4);
      uint offsetNL = read(data + offset + 26, 2);
      uint offsetEL = read(data + offset + 28, 2);
      file.data = data + offset + 30 + offsetNL + offsetEL;

      directory += 46 + namelength + extralength + commentlength;

      this->file.append(file);
    }

    return true;
  }

  auto extract(File& file) -> vector<uint8> {
    vector<uint8> buffer;

    if(file.cmode == 0) {
      buffer.resize(file.size);
      memcpy(buffer.data(), file.data, file.size);
    }

    if(file.cmode == 8) {
      buffer.resize(file.size);
      if(inflate(buffer.data(), buffer.size(), file.data, file.csize) == false) {
        buffer.reset();
      }
    }

    return buffer;
  }

  auto close() -> void {
    if(fm.open()) fm.close();
  }

protected:
  filemap fm;
  const uint8* filedata;
  uint filesize;

  auto read(const uint8* data, uint size) -> uint {
    uint result = 0, shift = 0;
    while(size--) { result |= *data++ << shift; shift += 8; }
    return result;
  }

public:
  vector<File> file;
};

}}
