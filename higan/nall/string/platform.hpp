#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

string activepath() {
  string result;
  #ifdef _WIN32
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
  #ifdef _WIN32
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

string userpath() {
  string result;
  #ifdef _WIN32
  wchar_t path[PATH_MAX] = L"";
  SHGetFolderPathW(0, CSIDL_APPDATA | CSIDL_FLAG_CREATE, 0, 0, path);
  result = (const char*)utf8_t(path);
  result.transform("\\", "/");
  #else
  char path[PATH_MAX] = "";
  struct passwd *userinfo = getpwuid(getuid());
  if(userinfo) strcpy(path, userinfo->pw_dir);
  result = path;
  #endif
  if(result.empty()) result = ".";
  if(result.endswith("/") == false) result.append("/");
  return result;
}

string configpath() {
  #ifdef _WIN32
  return userpath();
  #else
  return {userpath(), ".config/"};
  #endif
}

string temppath() {
  #ifdef _WIN32
  wchar_t path[PATH_MAX] = L"";
  GetTempPathW(PATH_MAX, path);
//path.transform("\\", "/");
  return (const char*)utf8_t(path);
  #else
  return "/tmp/";
  #endif
}

}

#endif
