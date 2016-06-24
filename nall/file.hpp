#pragma once

#include <nall/platform.hpp>
#include <nall/inode.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>
#include <nall/utility.hpp>
#include <nall/varint.hpp>
#include <nall/hash/sha256.hpp>

namespace nall {

struct file : inode, varint {
  enum class mode : uint { read, write, modify, append, readwrite = modify, writeread = append };
  enum class index : uint { absolute, relative };

  static auto copy(const string& sourcename, const string& targetname) -> bool {
    if(sourcename == targetname) return true;
    file rd, wr;
    if(rd.open(sourcename, mode::read) == false) return false;
    if(wr.open(targetname, mode::write) == false) return false;
    for(uint n = 0; n < rd.size(); n++) wr.write(rd.read());
    return true;
  }

  //attempt to rename file first
  //this will fail if paths point to different file systems; fall back to copy+remove in this case
  static auto move(const string& sourcename, const string& targetname) -> bool {
    if(sourcename == targetname) return true;
    if(rename(sourcename, targetname)) return true;
    if(!writable(sourcename)) return false;
    if(copy(sourcename, targetname)) {
      remove(sourcename);
      return true;
    }
    return false;
  }

  static auto truncate(const string& filename, uint size) -> bool {
    #if defined(API_POSIX)
    return truncate(filename, size) == 0;
    #elif defined(API_WINDOWS)
    if(auto fp = _wfopen(utf16_t(filename), L"rb+")) {
      bool result = _chsize(fileno(fp), size) == 0;
      fclose(fp);
      return result;
    }
    return false;
    #endif
  }

  //returns false if specified filename is a directory
  static auto exists(const string& filename) -> bool {
    #if defined(API_POSIX)
    struct stat data;
    if(stat(filename, &data) != 0) return false;
    #elif defined(API_WINDOWS)
    struct __stat64 data;
    if(_wstat64(utf16_t(filename), &data) != 0) return false;
    #endif
    return !(data.st_mode & S_IFDIR);
  }

  static auto size(const string& filename) -> uintmax {
    #if defined(API_POSIX)
    struct stat data;
    stat(filename, &data);
    #elif defined(API_WINDOWS)
    struct __stat64 data;
    _wstat64(utf16_t(filename), &data);
    #endif
    return S_ISREG(data.st_mode) ? data.st_size : 0u;
  }

  static auto read(const string& filename) -> vector<uint8_t> {
    vector<uint8_t> memory;
    file fp;
    if(fp.open(filename, mode::read)) {
      memory.resize(fp.size());
      fp.read(memory.data(), memory.size());
    }
    return memory;
  }

  static auto read(const string& filename, uint8_t* data, uint size) -> bool {
    file fp;
    if(fp.open(filename, mode::read) == false) return false;
    fp.read(data, size);
    fp.close();
    return true;
  }

  static auto write(const string& filename, const string& text) -> bool {
    return write(filename, (const uint8_t*)text.data(), text.size());
  }

  static auto write(const string& filename, const vector<uint8_t>& buffer) -> bool {
    return write(filename, buffer.data(), buffer.size());
  }

  static auto write(const string& filename, const uint8_t* data, uint size) -> bool {
    file fp;
    if(fp.open(filename, mode::write) == false) return false;
    fp.write(data, size);
    fp.close();
    return true;
  }

  static auto create(const string& filename) -> bool {
    //create an empty file (will replace existing files)
    file fp;
    if(fp.open(filename, mode::write) == false) return false;
    fp.close();
    return true;
  }

  static auto sha256(const string& filename) -> string {
    auto buffer = read(filename);
    return Hash::SHA256(buffer.data(), buffer.size()).digest();
  }

  auto read() -> uint8_t {
    if(!fp) return 0xff;                       //file not open
    if(file_mode == mode::write) return 0xff;  //reads not permitted
    if(file_offset >= file_size) return 0xff;  //cannot read past end of file
    buffer_sync();
    return buffer[(file_offset++) & buffer_mask];
  }

  auto readl(uint length = 1) -> uintmax {
    uintmax data = 0;
    for(int i = 0; i < length; i++) {
      data |= (uintmax)read() << (i << 3);
    }
    return data;
  }

  auto readm(uint length = 1) -> uintmax {
    uintmax data = 0;
    while(length--) {
      data <<= 8;
      data |= read();
    }
    return data;
  }

  auto reads(uint length) -> string {
    string result;
    result.resize(length);
    for(auto& byte : result) byte = read();
    return result;
  }

  auto read(uint8_t* buffer, uint length) -> void {
    while(length--) *buffer++ = read();
  }

  auto write(uint8_t data) -> void {
    if(!fp) return;                      //file not open
    if(file_mode == mode::read) return;  //writes not permitted
    buffer_sync();
    buffer[(file_offset++) & buffer_mask] = data;
    buffer_dirty = true;
    if(file_offset > file_size) file_size = file_offset;
  }

  auto writel(uintmax data, uint length = 1) -> void {
    while(length--) {
      write(data);
      data >>= 8;
    }
  }

  auto writem(uintmax data, uint length = 1) -> void {
    for(int i = length - 1; i >= 0; i--) {
      write(data >> (i << 3));
    }
  }

  auto writes(const string& s) -> void {
    for(auto byte : s) write(byte);
  }

  auto write(const uint8_t* buffer, uint length) -> void {
    while(length--) write(*buffer++);
  }

  template<typename... Args> auto print(Args... args) -> void {
    string data(args...);
    const char* p = data;
    while(*p) write(*p++);
  }

  auto flush() -> void {
    buffer_flush();
    fflush(fp);
  }

  auto seek(int offset, index index_ = index::absolute) -> void {
    if(!fp) return;  //file not open
    buffer_flush();

    intmax req_offset = file_offset;
    switch(index_) {
    case index::absolute: req_offset  = offset; break;
    case index::relative: req_offset += offset; break;
    }

    if(req_offset < 0) req_offset = 0;  //cannot seek before start of file
    if(req_offset > file_size) {
      if(file_mode == mode::read) {     //cannot seek past end of file
        req_offset = file_size;
      } else {                          //pad file to requested location
        file_offset = file_size;
        while(file_size < req_offset) write(0x00);
      }
    }

    file_offset = req_offset;
  }

  auto offset() const -> uint {
    if(!fp) return 0;  //file not open
    return file_offset;
  }

  auto size() const -> uint {
    if(!fp) return 0;  //file not open
    return file_size;
  }

  auto truncate(uint size) -> bool {
    if(!fp) return false;  //file not open
    #if defined(API_POSIX)
    return ftruncate(fileno(fp), size) == 0;
    #elif defined(API_WINDOWS)
    return _chsize(fileno(fp), size) == 0;
    #endif
  }

  auto end() const -> bool {
    if(!fp) return true;  //file not open
    return file_offset >= file_size;
  }

  auto open() const -> bool {
    return fp;
  }

  explicit operator bool() const {
    return open();
  }

  auto open(const string& filename, mode mode_) -> bool {
    if(fp) return false;

    switch(file_mode = mode_) {
    #if defined(API_POSIX)
    case mode::read:      fp = fopen(filename, "rb" ); break;
    case mode::write:     fp = fopen(filename, "wb+"); break;  //need read permission for buffering
    case mode::readwrite: fp = fopen(filename, "rb+"); break;
    case mode::writeread: fp = fopen(filename, "wb+"); break;
    #elif defined(API_WINDOWS)
    case mode::read:      fp = _wfopen(utf16_t(filename), L"rb" ); break;
    case mode::write:     fp = _wfopen(utf16_t(filename), L"wb+"); break;
    case mode::readwrite: fp = _wfopen(utf16_t(filename), L"rb+"); break;
    case mode::writeread: fp = _wfopen(utf16_t(filename), L"wb+"); break;
    #endif
    }
    if(!fp) return false;
    buffer_offset = -1;  //invalidate buffer
    file_offset = 0;
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return true;
  }

  auto close() -> void {
    if(!fp) return;
    buffer_flush();
    fclose(fp);
    fp = nullptr;
  }

  auto operator=(const file&) -> file& = delete;
  file(const file&) = delete;
  file() = default;

  file(const string& filename, mode mode_) {
    open(filename, mode_);
  }

  ~file() {
    close();
  }

private:
  enum { buffer_size = 1 << 12, buffer_mask = buffer_size - 1 };
  char buffer[buffer_size] = {0};
  int buffer_offset = -1;  //invalidate buffer
  bool buffer_dirty = false;
  FILE* fp = nullptr;
  uint file_offset = 0;
  uint file_size = 0;
  mode file_mode = mode::read;

  auto buffer_sync() -> void {
    if(!fp) return;  //file not open
    if(buffer_offset != (file_offset & ~buffer_mask)) {
      buffer_flush();
      buffer_offset = file_offset & ~buffer_mask;
      fseek(fp, buffer_offset, SEEK_SET);
      uint length = (buffer_offset + buffer_size) <= file_size ? buffer_size : (file_size & buffer_mask);
      if(length) auto unused = fread(buffer, 1, length, fp);
    }
  }

  auto buffer_flush() -> void {
    if(!fp) return;                      //file not open
    if(file_mode == mode::read) return;  //buffer cannot be written to
    if(buffer_offset < 0) return;        //buffer unused
    if(buffer_dirty == false) return;    //buffer unmodified since read
    fseek(fp, buffer_offset, SEEK_SET);
    uint length = (buffer_offset + buffer_size) <= file_size ? buffer_size : (file_size & buffer_mask);
    if(length) auto unused = fwrite(buffer, 1, length, fp);
    buffer_offset = -1;                  //invalidate buffer
    buffer_dirty = false;
  }
};

}
