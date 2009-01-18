#ifndef NALL_UTF8_HPP
#define NALL_UTF8_HPP

#include <nall/new.hpp>

//UTF-8 <> UTF-16 conversion
//used only for Win32; Linux, etc use UTF-8 internally

#if defined(_WIN32)

#undef  NOMINMAX
#define NOMINMAX
#include <windows.h>

namespace nall {
  //UTF-8 to UTF-16
  class utf16 {
  public:
    operator wchar_t*() {
      return buffer;
    }

    operator const wchar_t*() const {
      return buffer;
    }

    utf16(const char *s = "") {
      if(!s) s = "";
      unsigned length = MultiByteToWideChar(CP_UTF8, 0, s, -1, 0, 0);
      buffer = new(zeromemory) wchar_t[length + 1];
      MultiByteToWideChar(CP_UTF8, 0, s, -1, buffer, length);
    }

    ~utf16() {
      delete[] buffer;
    }

  private:
    wchar_t *buffer;
  };

  //UTF-16 to UTF-8
  class utf8 {
  public:
    operator char*() {
      return buffer;
    }

    operator const char*() const {
      return buffer;
    }

    utf8(const wchar_t *s = L"") {
      if(!s) s = L"";
      unsigned length = WideCharToMultiByte(CP_UTF8, 0, s, -1, 0, 0, (const char*)0, (BOOL*)0);
      buffer = new(zeromemory) char[length + 1];
      WideCharToMultiByte(CP_UTF8, 0, s, -1, buffer, length, (const char*)0, (BOOL*)0);
    }

    ~utf8() {
      delete[] buffer;
    }

  private:
    char *buffer;
  };
}

#endif  //if defined(_WIN32)

#endif
