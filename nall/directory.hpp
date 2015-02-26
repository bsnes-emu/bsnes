#ifndef NALL_DIRECTORY_HPP
#define NALL_DIRECTORY_HPP

#include <nall/file.hpp>
#include <nall/intrinsics.hpp>
#include <nall/sort.hpp>
#include <nall/storage.hpp>
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

struct directory : storage {
  static bool create(const string& pathname, unsigned permissions = 0755);  //recursive
  static bool remove(const string& pathname);  //recursive
  static bool exists(const string& pathname);

  static lstring folders(const string& pathname, const string& pattern = "*") {
    lstring folders = directory::ufolders(pathname, pattern);
    folders.sort();
    return folders;
  }

  static lstring files(const string& pathname, const string& pattern = "*") {
    lstring files = directory::ufiles(pathname, pattern);
    files.sort();
    return files;
  }

  static lstring contents(const string& pathname, const string& pattern = "*") {
    lstring folders = directory::ufolders(pathname);  //pattern search of contents should only filter files
    lstring files = directory::ufiles(pathname, pattern);
    folders.sort();
    files.sort();
    for(auto& file : files) folders.append(file);
    return folders;
  }

  static lstring ifolders(const string& pathname, const string& pattern = "*") {
    lstring folders = ufolders(pathname, pattern);
    folders.isort();
    return folders;
  }

  static lstring ifiles(const string& pathname, const string& pattern = "*") {
    lstring files = ufiles(pathname, pattern);
    files.isort();
    return files;
  }

  static lstring icontents(const string& pathname, const string& pattern = "*") {
    lstring folders = directory::ufolders(pathname);  //pattern search of contents should only filter files
    lstring files = directory::ufiles(pathname, pattern);
    folders.isort();
    files.isort();
    for(auto& file : files) folders.append(file);
    return folders;
  }

private:
  //internal functions; these return unsorted lists
  static lstring ufolders(const string& pathname, const string& pattern = "*");
  static lstring ufiles(const string& pathname, const string& pattern = "*");
};

#if defined(PLATFORM_WINDOWS)
  inline bool directory::create(const string& pathname, unsigned permissions) {
    string path;
    lstring list = string{pathname}.transform("\\", "/").rtrim("/").split("/");
    bool result = true;
    for(auto& part : list) {
      path.append(part, "/");
      result &= (_wmkdir(utf16_t(path)) == 0);
    }
    return result;
  }

  inline bool directory::remove(const string& pathname) {
    lstring list = directory::contents(pathname);
    for(auto& name : list) {
      if(name.endsWith("/")) directory::remove({pathname, name});
      else file::remove({pathname, name});
    }
    return _wrmdir(utf16_t(pathname)) == 0;
  }

  inline bool directory::exists(const string& pathname) {
    string name = pathname;
    name.trim("\"", "\"");
    DWORD result = GetFileAttributes(utf16_t(name));
    if(result == INVALID_FILE_ATTRIBUTES) return false;
    return (result & FILE_ATTRIBUTE_DIRECTORY);
  }

  inline lstring directory::ufolders(const string& pathname, const string& pattern) {
    lstring list;
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

  inline lstring directory::ufiles(const string& pathname, const string& pattern) {
    lstring list;
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
  inline bool directory_is_folder(DIR* dp, struct dirent* ep) {
    if(ep->d_type == DT_DIR) return true;
    if(ep->d_type == DT_LNK || ep->d_type == DT_UNKNOWN) {
      //symbolic links must be resolved to determine type
      struct stat sp = {0};
      fstatat(dirfd(dp), ep->d_name, &sp, 0);
      return S_ISDIR(sp.st_mode);
    }
    return false;
  }

  inline bool directory::create(const string& pathname, unsigned permissions) {
    string path;
    lstring list = string{pathname}.rtrim("/").split("/");
    bool result = true;
    for(auto& part : list) {
      path.append(part, "/");
      if(directory::exists(path)) continue;
      result &= (mkdir(path, permissions) == 0);
    }
    return result;
  }

  inline bool directory::remove(const string& pathname) {
    lstring list = directory::contents(pathname);
    for(auto& name : list) {
      if(name.endsWith("/")) directory::remove({pathname, name});
      else file::remove({pathname, name});
    }
    return rmdir(pathname) == 0;
  }

  inline bool directory::exists(const string& pathname) {
    DIR* dp = opendir(pathname);
    if(!dp) return false;
    closedir(dp);
    return true;
  }

  inline lstring directory::ufolders(const string& pathname, const string& pattern) {
    lstring list;
    DIR* dp;
    struct dirent* ep;
    dp = opendir(pathname);
    if(dp) {
      while(ep = readdir(dp)) {
        if(!strcmp(ep->d_name, ".")) continue;
        if(!strcmp(ep->d_name, "..")) continue;
        if(!directory_is_folder(dp, ep)) continue;
        string name{ep->d_name};
        if(name.match(pattern)) list.append(std::move(name));
      }
      closedir(dp);
    }
    for(auto& name : list) name.append("/");  //must append after sorting
    return list;
  }

  inline lstring directory::ufiles(const string& pathname, const string& pattern) {
    lstring list;
    DIR* dp;
    struct dirent* ep;
    dp = opendir(pathname);
    if(dp) {
      while(ep = readdir(dp)) {
        if(!strcmp(ep->d_name, ".")) continue;
        if(!strcmp(ep->d_name, "..")) continue;
        if(directory_is_folder(dp, ep)) continue;
        string name{ep->d_name};
        if(name.match(pattern)) list.append(std::move(name));
      }
      closedir(dp);
    }
    return list;
  }
#endif

}

#endif
