#ifndef NALL_DL_HPP
#define NALL_DL_HPP

//dynamic linking support

#include <nall/intrinsics.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>
#include <nall/utility.hpp>

#if defined(PLATFORM_XORG) || defined(PLATFORM_MACOSX)
  #include <dlfcn.h>
#elif defined(PLATFORM_WINDOWS)
  #include <windows.h>
  #include <nall/windows/utf8.hpp>
#endif

namespace nall {

struct library {
  explicit operator bool() const { return open(); }
  bool open() const { return handle; }
  bool open(const string&, const string& = "");
  bool open_absolute(const string&);
  void* sym(const string&);
  void close();

  library() = default;
  ~library() { close(); }

  library& operator=(const library&) = delete;
  library(const library&) = delete;

private:
  uintptr_t handle = 0;
};

#if defined(PLATFORM_XORG)
inline bool library::open(const string& name, const string& path) {
  if(handle) close();
  handle = (uintptr_t)dlopen(string(path, !path.empty() && !path.endsWith("/") ? "/" : "", "lib", name, ".so"), RTLD_LAZY);
  if(!handle) handle = (uintptr_t)dlopen(string("/usr/local/lib/lib", name, ".so"), RTLD_LAZY);
  return handle;
}

inline bool library::open_absolute(const string& name) {
  if(handle) close();
  handle = (uintptr_t)dlopen(name, RTLD_LAZY);
  return handle;
}

inline void* library::sym(const string& name) {
  if(!handle) return nullptr;
  return dlsym((void*)handle, name);
}

inline void library::close() {
  if(!handle) return;
  dlclose((void*)handle);
  handle = 0;
}
#elif defined(PLATFORM_MACOSX)
inline bool library::open(const string& name, const string& path) {
  if(handle) close();
  handle = (uintptr_t)dlopen(string(path, !path.empty() && !path.endsWith("/") ? "/" : "", "lib", name, ".dylib"), RTLD_LAZY);
  if(!handle) handle = (uintptr_t)dlopen(string("/usr/local/lib/lib", name, ".dylib"), RTLD_LAZY);
  return handle;
}

inline bool library::open_absolute(const string& name) {
  if(handle) close();
  handle = (uintptr_t)dlopen(name, RTLD_LAZY);
  return handle;
}

inline void* library::sym(const string& name) {
  if(!handle) return nullptr;
  return dlsym((void*)handle, name);
}

inline void library::close() {
  if(!handle) return;
  dlclose((void*)handle);
  handle = 0;
}
#elif defined(PLATFORM_WINDOWS)
inline bool library::open(const string& name, const string& path) {
  if(handle) close();
  string filepath(path, !path.empty() && !path.endsWith("/") && !path.endsWith("\\") ? "/" : "", name, ".dll");
  handle = (uintptr_t)LoadLibraryW(utf16_t(filepath));
  return handle;
}

inline bool library::open_absolute(const string& name) {
  if(handle) close();
  handle = (uintptr_t)LoadLibraryW(utf16_t(name));
  return handle;
}

inline void* library::sym(const string& name) {
  if(!handle) return nullptr;
  return (void*)GetProcAddress((HMODULE)handle, name);
}

inline void library::close() {
  if(!handle) return;
  FreeLibrary((HMODULE)handle);
  handle = 0;
}
#else
inline bool library::open(const string&, const string&) { return false; }
inline bool library::open_absolute(const string&) { return false; }
inline void* library::sym(const string&) { return nullptr; }
inline void library::close() {}
#endif

}

#endif
