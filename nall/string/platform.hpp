#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

string activepath() {
  char path[PATH_MAX] = "";
  auto unused = getcwd(path, PATH_MAX);
  string result = path;
  if(result.empty()) result = ".";
  result.transform("\\", "/");
  if(result.endsWith("/") == false) result.append("/");
  return result;
}

string realpath(const string& name) {
  string result;
  char path[PATH_MAX] = "";
  if(::realpath(name, path)) result = dir(path);
  if(result.empty()) result = activepath();
  result.transform("\\", "/");
  if(result.endsWith("/") == false) result.append("/");
  return result;
}

// /home/username/
// c:/users/username/
string userpath() {
  #if defined(PLATFORM_WINDOWS)
  wchar_t path[PATH_MAX] = L"";
  SHGetFolderPathW(nullptr, CSIDL_PROFILE | CSIDL_FLAG_CREATE, nullptr, 0, path);
  string result = (const char*)utf8_t(path);
  result.transform("\\", "/");
  #else
  struct passwd* userinfo = getpwuid(getuid());
  string result = userinfo->pw_dir;
  #endif
  if(result.empty()) result = ".";
  if(result.endsWith("/") == false) result.append("/");
  return result;
}

// /home/username/.config/
// c:/users/username/appdata/roaming/
string configpath() {
  #if defined(PLATFORM_WINDOWS)
  wchar_t path[PATH_MAX] = L"";
  SHGetFolderPathW(nullptr, CSIDL_APPDATA | CSIDL_FLAG_CREATE, nullptr, 0, path);
  string result = (const char*)utf8_t(path);
  result.transform("\\", "/");
  #elif defined(PLATFORM_MACOSX)
  string result = {userpath(), "Library/Application Support/"};
  #else
  string result = {userpath(), ".config/"};
  #endif
  if(result.empty()) result = ".";
  if(result.endsWith("/") == false) result.append("/");
  return result;
}

// /usr/share
// /Library/Application Support/
// c:/ProgramData/
string sharedpath() {
  #if defined(PLATFORM_WINDOWS)
  wchar_t path[PATH_MAX] = L"";
  SHGetFolderPathW(nullptr, CSIDL_COMMON_APPDATA | CSIDL_FLAG_CREATE, nullptr, 0, path);
  string result = (const char*)utf8_t(path);
  result.transform("\\", "/");
  #elif defined(PLATFORM_MACOSX)
  string result = "/Library/Application Support/";
  #else
  string result = "/usr/share/";
  #endif
  if(result.empty()) result = ".";
  if(result.endsWith("/") == false) result.append("/");
  return result;
}

// /tmp
// c:/users/username/AppData/Local/Temp/
string temppath() {
  #if defined(PLATFORM_WINDOWS)
  wchar_t path[PATH_MAX] = L"";
  GetTempPathW(PATH_MAX, path);
  string result = (const char*)utf8_t(path);
  result.transform("\\", "/");
  #elif defined(P_tmpdir)
  string result = P_tmpdir;
  #else
  string result = "/tmp/";
  #endif
  if(result.endsWith("/") == false) result.append("/");
  return result;
}

}

#endif
