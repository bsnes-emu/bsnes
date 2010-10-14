#ifndef NALL_DIRECTORY_HPP
#define NALL_DIRECTORY_HPP

#include <nall/foreach.hpp>
#include <nall/sort.hpp>
#include <nall/string.hpp>

#if defined(_WIN32)
  #include <nall/utf8.hpp>
#else
  #include <dirent.h>
  #include <stdio.h>
  #include <sys/types.h>
#endif

namespace nall {

struct directory {
  static lstring folders(const string &pathname, const string &pattern = "*");
  static lstring files(const string &pathname, const string &pattern = "*");
  static lstring contents(const string &pathname, const string &pattern = "*");
};

#if defined(_WIN32)
  inline lstring directory::folders(const string &pathname, const string &pattern) {
    lstring list;
    string path = pathname;
    path.transform("/", "\\");
    if(!strend(path, "\\")) path.append("\\");
    path.append("*");
    HANDLE handle;
    WIN32_FIND_DATA data;
    handle = FindFirstFile(utf16_t(path), &data);
    if(handle != INVALID_HANDLE_VALUE) {
      if(wcscmp(data.cFileName, L".") && wcscmp(data.cFileName, L"..")) {
        if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
          string name = utf8_t(data.cFileName);
          if(wildcard(name, pattern)) list.append(string(name, "/"));
        }
      }
      while(FindNextFile(handle, &data) != false) {
        if(wcscmp(data.cFileName, L".") && wcscmp(data.cFileName, L"..")) {
          if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            string name = utf8_t(data.cFileName);
            if(wildcard(name, pattern)) list.append(string(name, "/"));
          }
        }
      }
      FindClose(handle);
    }
    if(list.size() > 0) sort(&list[0], list.size());
    return list;
  }

  inline lstring directory::files(const string &pathname, const string &pattern) {
    lstring list;
    string path = pathname;
    path.transform("/", "\\");
    if(!strend(path, "\\")) path.append("\\");
    path.append("*");
    HANDLE handle;
    WIN32_FIND_DATA data;
    handle = FindFirstFile(utf16_t(path), &data);
    if(handle != INVALID_HANDLE_VALUE) {
      if((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
        string name = utf8_t(data.cFileName);
        if(wildcard(name, pattern)) list.append(name);
      }
      while(FindNextFile(handle, &data) != false) {
        if((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
          string name = utf8_t(data.cFileName);
          if(wildcard(name, pattern)) list.append(name);
        }
      }
      FindClose(handle);
    }
    if(list.size() > 0) sort(&list[0], list.size());
    return list;
  }

  inline lstring directory::contents(const string &pathname, const string &pattern) {
    lstring folders = directory::folders(pathname);  //pattern search of contents() should only filter files
    lstring files = directory::files(pathname, pattern);
    foreach(file, files) folders.append(file);
    return folders;
  }
#else
  inline lstring directory::folders(const string &pathname, const string &pattern) {
    lstring list;
    DIR *dp;
    struct dirent *ep;
    dp = opendir(pathname);
    if(dp) {
      while(ep = readdir(dp)) {
        if(!strcmp(ep->d_name, ".")) continue;
        if(!strcmp(ep->d_name, "..")) continue;
        if(ep->d_type & DT_DIR) {
          if(wildcard(ep->d_name, pattern)) list.append(string(ep->d_name, "/"));
        }
      }
      closedir(dp);
    }
    if(list.size() > 0) sort(&list[0], list.size());
    return list;

  }

  inline lstring directory::files(const string &pathname, const string &pattern) {
    lstring list;
    DIR *dp;
    struct dirent *ep;
    dp = opendir(pathname);
    if(dp) {
      while(ep = readdir(dp)) {
        if(!strcmp(ep->d_name, ".")) continue;
        if(!strcmp(ep->d_name, "..")) continue;
        if((ep->d_type & DT_DIR) == 0) {
          if(wildcard(ep->d_name, pattern)) list.append(ep->d_name);
        }
      }
      closedir(dp);
    }
    if(list.size() > 0) sort(&list[0], list.size());
    return list;
  }

  inline lstring directory::contents(const string &pathname, const string &pattern) {
    lstring folders = directory::folders(pathname);  //pattern search of contents() should only filter files
    lstring files = directory::files(pathname, pattern);
    foreach(file, files) folders.append(file);
    return folders;
  }
#endif

}

#endif
