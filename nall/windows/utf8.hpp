#pragma once

//UTF-8 <> UTF-16 conversion
//used only for Win32; every other OS uses UTF-8 internally

#if defined(_WIN32)

#undef  UNICODE
#define UNICODE
#undef  NOMINMAX
#define NOMINMAX

#include <nall/windows/guard.hpp>
#include <winsock2.h>
#include <windows.h>
#include <nall/windows/guard.hpp>

#if !defined(PATH_MAX)
  #define PATH_MAX 260
#endif

using uint = unsigned;

namespace nall {
  //UTF-8 to UTF-16
  struct utf16_t {
    utf16_t(const char* s = "") {
      if(!s) s = "";
      uint length = MultiByteToWideChar(CP_UTF8, 0, s, -1, nullptr, 0);
      buffer = new wchar_t[length + 1]();
      MultiByteToWideChar(CP_UTF8, 0, s, -1, buffer, length);
    }

    ~utf16_t() {
      delete[] buffer;
    }

    operator wchar_t*() {
      return buffer;
    }

    operator const wchar_t*() const {
      return buffer;
    }

  private:
    wchar_t* buffer = nullptr;
  };

  //UTF-16 to UTF-8
  struct utf8_t {
    utf8_t(const wchar_t* s = L"") {
      if(!s) s = L"";
      uint length = WideCharToMultiByte(CP_UTF8, 0, s, -1, nullptr, 0, nullptr, nullptr);
      buffer = new char[length + 1]();
      WideCharToMultiByte(CP_UTF8, 0, s, -1, buffer, length, nullptr, nullptr);
    }

    ~utf8_t() {
      delete[] buffer;
    }

    utf8_t(const utf8_t&) = delete;
    utf8_t& operator=(const utf8_t&) = delete;

    operator char*() {
      return buffer;
    }

    operator const char*() const {
      return buffer;
    }

  private:
    char* buffer = nullptr;
  };

  inline auto utf8_args(int& argc, char**& argv) -> void {
    wchar_t** wargv = CommandLineToArgvW(GetCommandLineW(), &argc);
    argv = new char*[argc];
    for(uint i = 0; i < argc; i++) {
      argv[i] = new char[PATH_MAX];
      strcpy(argv[i], nall::utf8_t(wargv[i]));
    }
  }
}

#endif  //if defined(_WIN32)
