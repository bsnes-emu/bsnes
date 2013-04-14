#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

string activepath() {
  string result;
  #if defined(PLATFORM_WINDOWS)
  wchar_t path[PATH_MAX] = L"";
  auto unused = _wgetcwd(path, PATH_MAX);
  result = (const char*)utf8_t(path);
  result.transform("\\", "/");
  #else
  char path[PATH_MAX] = "";
  auto unused = getcwd(path, PATH_MAX);
  result = path;
  #endif
  if(result.empty()) result = ".";
  if(result.endswith("/") == false) result.append("/");
  return result;
}

string realpath(const string &name) {
  string result;
  #if defined(PLATFORM_WINDOWS)
  wchar_t path[PATH_MAX] = L"";
  if(_wfullpath(path, utf16_t(name), PATH_MAX)) result = (const char*)utf8_t(path);
  result.transform("\\", "/");
  #else
  char path[PATH_MAX] = "";
  if(::realpath(name, path)) result = path;
  #endif
  if(result.empty()) result = {activepath(), name};
  return result;
}

// /home/username/
// c:/users/username/
string userpath() {
  string result;
  #if defined(PLATFORM_WINDOWS)
  wchar_t path[PATH_MAX] = L"";
  SHGetFolderPathW(nullptr, CSIDL_PROFILE | CSIDL_FLAG_CREATE, nullptr, 0, path);
  result = (const char*)utf8_t(path);
  result.transform("\\", "/");
  #else
  struct passwd *userinfo = getpwuid(getuid());
  result = userinfo->pw_dir;
  #endif
  if(result.empty()) result = ".";
  if(result.endswith("/") == false) result.append("/");
  return result;
}

// /home/username/.config/
// c:/users/username/appdata/roaming/
string configpath() {
  string result;
  #if defined(PLATFORM_WINDOWS)
  wchar_t path[PATH_MAX] = L"";
  SHGetFolderPathW(nullptr, CSIDL_APPDATA | CSIDL_FLAG_CREATE, nullptr, 0, path);
  result = (const char*)utf8_t(path);
  result.transform("\\", "/");
  #elif defined(PLATFORM_OSX)
  result = {userpath(), "Library/Application Support/"};
  #else
  result = {userpath(), ".config/"};
  #endif
  if(result.empty()) result = ".";
  if(result.endswith("/") == false) result.append("/");
  return result;
}

string sharedpath() {
  string result;
  #if defined(PLATFORM_WINDOWS)
  wchar_t path[PATH_MAX] = L"";
  SHGetFolderPathW(nullptr, CSIDL_COMMON_APPDATA | CSIDL_FLAG_CREATE, nullptr, 0, path);
  result = (const char*)utf8_t(path);
  result.transform("\\", "/");
  #elif defined(PLATFORM_OSX)
  result = "/Library/Application Support/";
  #else
  result = "/usr/share/";
  #endif
  if(result.empty()) result = ".";
  if(result.endswith("/") == false) result.append("/");
  return result;
}

string temppath() {
  #if defined(PLATFORM_WINDOWS)
  wchar_t path[PATH_MAX] = L"";
  GetTempPathW(PATH_MAX, path);
  string result = (const char*)utf8_t(path);
  result.transform("\\", "/");
  return result;
  #else
  return "/tmp/";
  #endif
}

}

#endif
