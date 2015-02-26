#ifndef NALL_STORAGE_HPP
#define NALL_STORAGE_HPP

//generic abstraction layer for common storage operations against both files and directories
//these functions are not recursive; use directory::create() and directory::remove() for recursion

#include <nall/platform.hpp>
#include <nall/string.hpp>

namespace nall {

struct storage {
  enum class time : unsigned { access, modify };

  static auto exists(const string& name) -> bool;
  static auto readable(const string& name) -> bool;
  static auto writable(const string& name) -> bool;
  static auto executable(const string& name) -> bool;
  static auto uid(const string& name) -> unsigned;
  static auto gid(const string& name) -> unsigned;
  static auto mode(const string& name) -> unsigned;
  static auto timestamp(const string& name, storage::time mode = storage::time::modify) -> time_t;

  static auto create(const string& name, unsigned permissions = 0755) -> bool;
  static auto rename(const string& name, const string& targetname) -> bool;
  static auto remove(const string& name) -> bool;
};

inline auto storage::exists(const string& name) -> bool {
  return access(name, F_OK) == 0;
}

inline auto storage::readable(const string& name) -> bool {
  return access(name, R_OK) == 0;
}

inline auto storage::writable(const string& name) -> bool {
  return access(name, W_OK) == 0;
}

inline auto storage::executable(const string& name) -> bool {
  return access(name, X_OK) == 0;
}

inline auto storage::uid(const string& name) -> unsigned {
  struct stat data = {0};
  stat(name, &data);
  return data.st_uid;
}

inline auto storage::gid(const string& name) -> unsigned {
  struct stat data = {0};
  stat(name, &data);
  return data.st_gid;
}

inline auto storage::mode(const string& name) -> unsigned {
  struct stat data = {0};
  stat(name, &data);
  return data.st_mode;
}

inline auto storage::timestamp(const string& name, storage::time mode) -> time_t {
  struct stat data = {0};
  stat(name, &data);
  switch(mode) {
  case storage::time::access: return data.st_atime;
  case storage::time::modify: return data.st_mtime;
  }
  throw;
}

//returns true if 'name' already exists
inline auto storage::create(const string& name, unsigned permissions) -> bool {
  if(storage::exists(name)) return true;
  if(name.endsWith("/")) return mkdir(name, permissions) == 0;
  int fd = open(name, O_CREAT | O_EXCL, permissions);
  if(fd < 0) return false;
  return close(fd), true;
}

//returns false if 'name' and 'targetname' are on different file systems (requires copy)
inline auto storage::rename(const string& name, const string& targetname) -> bool {
  return ::rename(name, targetname) == 0;
}

//returns false if 'name' is a directory that is not empty
inline auto storage::remove(const string& name) -> bool {
  if(name.endsWith("/")) return rmdir(name) == 0;
  return unlink(name) == 0;
}

}

#endif
