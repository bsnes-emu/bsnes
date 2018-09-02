#pragma once

//generic abstraction layer for common storage operations against both files and directories
//these functions are not recursive; use directory::create() and directory::remove() for recursion

#include <nall/platform.hpp>
#include <nall/string.hpp>

namespace nall {

struct inode {
  enum class time : uint { create, modify, access };

  static auto exists(const string& name) -> bool {
    return access(name, F_OK) == 0;
  }

  static auto readable(const string& name) -> bool {
    return access(name, R_OK) == 0;
  }

  static auto writable(const string& name) -> bool {
    return access(name, W_OK) == 0;
  }

  static auto executable(const string& name) -> bool {
    return access(name, X_OK) == 0;
  }

  static auto mode(const string& name) -> uint {
    struct stat data{};
    stat(name, &data);
    return data.st_mode;
  }

  static auto uid(const string& name) -> uint {
    struct stat data{};
    stat(name, &data);
    return data.st_uid;
  }

  static auto gid(const string& name) -> uint {
    struct stat data{};
    stat(name, &data);
    return data.st_gid;
  }

  #if !defined(PLATFORM_WINDOWS)
  static auto user(const string& name) -> string {
    struct passwd* pw = getpwuid(uid(name));
    if(pw && pw->pw_name) return pw->pw_name;
    return {};
  }

  static auto group(const string& name) -> string {
    struct group* gr = getgrgid(gid(name));
    if(gr && gr->gr_name) return gr->gr_name;
    return {};
  }
  #endif

  static auto timestamp(const string& name, time mode = time::modify) -> uint64_t {
    struct stat data{};
    stat(name, &data);
    switch(mode) {
    #if defined(PLATFORM_WINDOWS)
    case time::create: return data.st_ctime;
    #else
    //st_birthtime may return -1 or st_atime if it is not supported
    //the best that can be done in this case is to return st_mtime if it's older
    case time::create: return min((uint)data.st_birthtime, (uint)data.st_mtime);
    #endif
    case time::modify: return data.st_mtime;
    //for performance reasons, last access time is usually not enabled on various filesystems
    //ensure that the last access time is not older than the last modify time (eg for NTFS)
    case time::access: return max((uint)data.st_atime, data.st_mtime);
    }
    return 0;
  }

  //returns true if 'name' already exists
  static auto create(const string& name, uint permissions = 0755) -> bool {
    if(exists(name)) return true;
    if(name.endsWith("/")) return mkdir(name, permissions) == 0;
    int fd = open(name, O_CREAT | O_EXCL, permissions);
    if(fd < 0) return false;
    return close(fd), true;
  }

  //returns false if 'name' and 'targetname' are on different file systems (requires copy)
  static auto rename(const string& name, const string& targetname) -> bool {
    return ::rename(name, targetname) == 0;
  }

  //returns false if 'name' is a directory that is not empty
  static auto remove(const string& name) -> bool {
    #if defined(PLATFORM_WINDOWS)
    if(name.endsWith("/")) return _wrmdir(utf16_t(name)) == 0;
    return _wunlink(utf16_t(name)) == 0;
    #else
    if(name.endsWith("/")) return rmdir(name) == 0;
    return unlink(name) == 0;
    #endif
  }
};

}
