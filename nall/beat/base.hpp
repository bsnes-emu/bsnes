#ifndef NALL_BEAT_BASE_HPP
#define NALL_BEAT_BASE_HPP

namespace nall {

struct beatBase {
protected:
  file fp;
  uint32_t checksum;

  void ls(lstring& list, const string& path, const string& basepath) {
    lstring paths = directory::folders(path);
    for(auto& pathname : paths) {
      list.append(string{path, pathname}.ltrim<1>(basepath));
      ls(list, {path, pathname}, basepath);
    }

    lstring files = directory::files(path);
    for(auto& filename : files) {
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
      if(data == 0) return write(0x80 | x);
      write(x);
      data--;
    }
  }

  void writeString(const string& text) {
    unsigned length = text.length();
    for(unsigned n = 0; n < length; n++) write(text[n]);
  }

  void writeChecksum(uint32_t checksum) {
    write(checksum >>  0);
    write(checksum >>  8);
    write(checksum >> 16);
    write(checksum >> 24);
  }

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
    for(unsigned n = 0; n < length; n++) {
      text[n] = fp.read();
      checksum = crc32_adjust(checksum, text[n]);
    }
    text[length] = 0;
    return text;
  }

  bool readChecksum(uint32_t source) {
    uint32_t checksum = 0;
    checksum |= read() <<  0;
    checksum |= read() <<  8;
    checksum |= read() << 16;
    checksum |= read() << 24;
    return checksum == source;
  }
};

}

#endif
