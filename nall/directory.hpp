#pragma once

#include <nall/file.hpp>
#include <nall/inode.hpp>
#include <nall/intrinsics.hpp>
#include <nall/merge-sort.hpp>
#include <nall/string.hpp>
#include <nall/vector.hpp>

#if defined(PLATFORM_WINDOWS)
  #include <nall/windows/utf8.hpp>
#else
  #include <dirent.h>
  #include <stdio.h>
  #include <sys/types.h>
#endif

namespace nall {

struct directory : inode {
  static auto create(const string& pathname, uint permissions = 0755) -> bool;  //recursive
  static auto remove(const string& pathname) -> bool;  //recursive
  static auto exists(const string& pathname) -> bool;

  static auto folders(const string& pathname, const string& pattern = "*") -> vector<string> {
    auto folders = directory::ufolders(pathname, pattern);
    folders.sort();
    return folders;
  }

  static auto files(const string& pathname, const string& pattern = "*") -> vector<string> {
    auto files = directory::ufiles(pathname, pattern);
    files.sort();
    return files;
  }

  static auto contents(const string& pathname, const string& pattern = "*") -> vector<string> {
    auto folders = directory::ufolders(pathname);  //pattern search of contents should only filter files
    auto files = directory::ufiles(pathname, pattern);
    folders.sort();
    files.sort();
    for(auto& file : files) folders.append(file);
    return folders;
  }

  static auto ifolders(const string& pathname, const string& pattern = "*") -> vector<string> {
    auto folders = ufolders(pathname, pattern);
    folders.isort();
    return folders;
  }

  static auto ifiles(const string& pathname, const string& pattern = "*") -> vector<string> {
    auto files = ufiles(pathname, pattern);
    files.isort();
    return files;
  }

  static auto icontents(const string& pathname, const string& pattern = "*") -> vector<string> {
    auto folders = directory::ufolders(pathname);  //pattern search of contents should only filter files
    auto files = directory::ufiles(pathname, pattern);
    folders.isort();
    files.isort();
    for(auto& file : files) folders.append(file);
    return folders;
  }

private:
  //internal functions; these return unsorted lists
  static auto ufolders(const string& pathname, const string& pattern = "*") -> vector<string>;
  static auto ufiles(const string& pathname, const string& pattern = "*") -> vector<string>;
};

#if defined(PLATFORM_WINDOWS)
  inline auto directory::create(const string& pathname, uint permissions) -> bool {
    string path;
    auto list = string{pathname}.transform("\\", "/").trimRight("/").split("/");
    bool result = true;
    for(auto& part : list) {
      path.append(part, "/");
      if(directory::exists(path)) continue;
      result &= (_wmkdir(utf16_t(path)) == 0);
    }
    return result;
  }

  inline auto directory::remove(const string& pathname) -> bool {
    auto list = directory::contents(pathname);
    for(auto& name : list) {
      if(name.endsWith("/")) directory::remove({pathname, name});
      else file::remove({pathname, name});
    }
    return _wrmdir(utf16_t(pathname)) == 0;
  }

  inline auto directory::exists(const string& pathname) -> bool {
    string name = pathname;
    name.trim("\"", "\"");
    DWORD result = GetFileAttributes(utf16_t(name));
    if(result == INVALID_FILE_ATTRIBUTES) return false;
    return (result & FILE_ATTRIBUTE_DIRECTORY);
  }

  inline auto directory::ufolders(const string& pathname, const string& pattern) -> vector<string> {
    if(!pathname) {
      //special root pseudo-folder (return list of drives)
      wchar_t drives[PATH_MAX] = {0};
      GetLogicalDriveStrings(PATH_MAX, drives);
      wchar_t* p = drives;
      while(*p || *(p + 1)) {
        if(!*p) *p = ';';
        *p++;
      }
      return string{(const char*)utf8_t(drives)}.replace("\\", "/").split(";");
    }

    vector<string> list;
    string path = pathname;
    path.transform("/", "\\");
    if(!path.endsWith("\\")) path.append("\\");
    path.append("*");
    HANDLE handle;
    WIN32_FIND_DATA data;
    handle = FindFirstFile(utf16_t(path), &data);
    if(handle != INVALID_HANDLE_VALUE) {
      if(wcscmp(data.cFileName, L".") && wcscmp(data.cFileName, L"..")) {
        if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
          string name = (const char*)utf8_t(data.cFileName);
          if(name.match(pattern)) list.append(name);
        }
      }
      while(FindNextFile(handle, &data) != false) {
        if(wcscmp(data.cFileName, L".") && wcscmp(data.cFileName, L"..")) {
          if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            string name = (const char*)utf8_t(data.cFileName);
            if(name.match(pattern)) list.append(name);
          }
        }
      }
      FindClose(handle);
    }
    for(auto& name : list) name.append("/");  //must append after sorting
    return list;
  }

  inline auto directory::ufiles(const string& pathname, const string& pattern) -> vector<string> {
    if(!pathname) return {};

    vector<string> list;
    string path = pathname;
    path.transform("/", "\\");
    if(!path.endsWith("\\")) path.append("\\");
    path.append("*");
    HANDLE handle;
    WIN32_FIND_DATA data;
    handle = FindFirstFile(utf16_t(path), &data);
    if(handle != INVALID_HANDLE_VALUE) {
      if((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
        string name = (const char*)utf8_t(data.cFileName);
        if(name.match(pattern)) list.append(name);
      }
      while(FindNextFile(handle, &data) != false) {
        if((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
          string name = (const char*)utf8_t(data.cFileName);
          if(name.match(pattern)) list.append(name);
        }
      }
      FindClose(handle);
    }
    return list;
  }
#else
  inline auto directoryIsFolder(DIR* dp, struct dirent* ep) -> bool {
    if(ep->d_type == DT_DIR) return true;
    if(ep->d_type == DT_LNK || ep->d_type == DT_UNKNOWN) {
      //symbolic links must be resolved to determine type
      struct stat sp = {0};
      fstatat(dirfd(dp), ep->d_name, &sp, 0);
      return S_ISDIR(sp.st_mode);
    }
    return false;
  }

  inline auto directory::create(const string& pathname, uint permissions) -> bool {
    string path;
    auto list = string{pathname}.trimRight("/").split("/");
    bool result = true;
    for(auto& part : list) {
      path.append(part, "/");
      if(directory::exists(path)) continue;
      result &= (mkdir(path, permissions) == 0);
    }
    return result;
  }

  inline auto directory::remove(const string& pathname) -> bool {
    auto list = directory::contents(pathname);
    for(auto& name : list) {
      if(name.endsWith("/")) directory::remove({pathname, name});
      else file::remove({pathname, name});
    }
    return rmdir(pathname) == 0;
  }

  inline auto directory::exists(const string& pathname) -> bool {
    struct stat data;
    if(stat(pathname, &data) != 0) return false;
    return S_ISDIR(data.st_mode);
  }

  inline auto directory::ufolders(const string& pathname, const string& pattern) -> vector<string> {
    if(!pathname) return vector<string>{"/"};

    vector<string> list;
    DIR* dp;
    struct dirent* ep;
    dp = opendir(pathname);
    if(dp) {
      while(ep = readdir(dp)) {
        if(!strcmp(ep->d_name, ".")) continue;
        if(!strcmp(ep->d_name, "..")) continue;
        if(!directoryIsFolder(dp, ep)) continue;
        string name{ep->d_name};
        if(name.match(pattern)) list.append(std::move(name));
      }
      closedir(dp);
    }
    for(auto& name : list) name.append("/");  //must append after sorting
    return list;
  }

  inline auto directory::ufiles(const string& pathname, const string& pattern) -> vector<string> {
    if(!pathname) return {};

    vector<string> list;
    DIR* dp;
    struct dirent* ep;
    dp = opendir(pathname);
    if(dp) {
      while(ep = readdir(dp)) {
        if(!strcmp(ep->d_name, ".")) continue;
        if(!strcmp(ep->d_name, "..")) continue;
        if(directoryIsFolder(dp, ep)) continue;
        string name{ep->d_name};
        if(name.match(pattern)) list.append(std::move(name));
      }
      closedir(dp);
    }
    return list;
  }
#endif

}
