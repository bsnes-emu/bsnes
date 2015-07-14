#ifndef NALL_DL_HPP
#define NALL_DL_HPP

//dynamic linking support

#include <nall/intrinsics.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>
#include <nall/utility.hpp>

#if defined(PLATFORM_WINDOWS)
  #include <windows.h>
  #include <nall/windows/utf8.hpp>
#else
  #include <dlfcn.h>
#endif

namespace nall {

struct library {
  library() = default;
  ~library() { close(); }

  library& operator=(const library&) = delete;
  library(const library&) = delete;

  explicit operator bool() const { return open(); }
  auto open() const -> bool { return handle; }
  auto open(const string&, const string& = "") -> bool;
  auto openAbsolute(const string&) -> bool;
  auto sym(const string&) -> void*;
  auto close() -> void;

private:
  uintptr_t handle = 0;
};

#if defined(PLATFORM_LINUX) || defined(PLATFORM_BSD)
inline auto library::open(const string& name, const string& path) -> bool {
  if(handle) close();
  if(path) handle = (uintptr_t)dlopen(string(path, "lib", name, ".so"), RTLD_LAZY);
  if(!handle) handle = (uintptr_t)dlopen(string(userpath(), ".local/lib/lib", name, ".so"), RTLD_LAZY);
  if(!handle) handle = (uintptr_t)dlopen(string("/usr/local/lib/lib", name, ".so"), RTLD_LAZY);
  if(!handle) handle = (uintptr_t)dlopen(string("lib", name, ".so"), RTLD_LAZY);
  return handle;
}

inline auto library::openAbsolute(const string& name) -> bool {
  if(handle) close();
  handle = (uintptr_t)dlopen(name, RTLD_LAZY);
  return handle;
}

inline auto library::sym(const string& name) -> void* {
  if(!handle) return nullptr;
  return dlsym((void*)handle, name);
}

inline auto library::close() -> void {
  if(!handle) return;
  dlclose((void*)handle);
  handle = 0;
}
#elif defined(PLATFORM_MACOSX)
inline auto library::open(const string& name, const string& path) -> bool {
  if(handle) close();
  if(path) handle = (uintptr_t)dlopen(string(path, "lib", name, ".dylib"), RTLD_LAZY);
  if(!handle) handle = (uintptr_t)dlopen(string(userpath(), ".local/lib/lib", name, ".dylib"), RTLD_LAZY);
  if(!handle) handle = (uintptr_t)dlopen(string("/usr/local/lib/lib", name, ".dylib"), RTLD_LAZY);
  if(!handle) handle = (uintptr_t)dlopen(string("lib", name, ".dylib"), RTLD_LAZY);
  return handle;
}

inline auto library::openAbsolute(const string& name) -> bool {
  if(handle) close();
  handle = (uintptr_t)dlopen(name, RTLD_LAZY);
  return handle;
}

inline auto library::sym(const string& name) -> void* {
  if(!handle) return nullptr;
  return dlsym((void*)handle, name);
}

inline auto library::close() -> void {
  if(!handle) return;
  dlclose((void*)handle);
  handle = 0;
}
#elif defined(PLATFORM_WINDOWS)
inline auto library::open(const string& name, const string& path) -> bool {
  if(handle) close();
  if(path) {
    string filepath = {path, name, ".dll"};
    handle = (uintptr_t)LoadLibraryW(utf16_t(filepath));
  }
  if(!handle) {
    string filepath = {name, ".dll"};
    handle = (uintptr_t)LoadLibraryW(utf16_t(filepath));
  }
  return handle;
}

inline auto library::openAbsolute(const string& name) -> bool {
  if(handle) close();
  handle = (uintptr_t)LoadLibraryW(utf16_t(name));
  return handle;
}

inline auto library::sym(const string& name) -> void* {
  if(!handle) return nullptr;
  return (void*)GetProcAddress((HMODULE)handle, name);
}

inline auto library::close() -> void {
  if(!handle) return;
  FreeLibrary((HMODULE)handle);
  handle = 0;
}
#else
inline auto library::open(const string&, const string&) -> bool { return false; }
inline auto library::openAbsolute(const string&) -> bool { return false; }
inline auto library::sym(const string&) -> void* { return nullptr; }
inline auto library::close() -> void {}
#endif

}

#endif
