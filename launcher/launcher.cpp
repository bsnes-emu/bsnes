#include <nall/config.hpp>
#include <nall/detect.hpp>
#include <nall/file.hpp>
#include <nall/foreach.hpp>
#include <nall/platform.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>
using namespace nall;

#if defined(PLATFORM_X) || defined(PLATFORM_OSX)
  char* userpath(char *path) {
    *path = 0;
    struct passwd *userinfo = getpwuid(getuid());
    if(userinfo) strcpy(path, userinfo->pw_dir);
    return path;
  }

  char *getcwd(char *path) {
    return getcwd(path, PATH_MAX);
  }
#elif defined(PLATFORM_WIN)
  #include <nall/utf8.hpp>
  #include <process.h>
  #include <wchar.h>
  #include <windows.h>

  char* realpath(const char *filename, char *resolvedname) {
    wchar_t fn[_MAX_PATH] = L"";
    _wfullpath(fn, nall::utf16_t(filename), _MAX_PATH);
    strcpy(resolvedname, nall::utf8_t(fn));
    return resolvedname;
  }

  char* userpath(char *path) {
    wchar_t fp[_MAX_PATH] = L"";
    SHGetFolderPathW(0, CSIDL_APPDATA | CSIDL_FLAG_CREATE, 0, 0, fp);
    strcpy(path, nall::utf8_t(fp));
    return path;
  }

  char* getcwd(char *path) {
    wchar_t fp[_MAX_PATH] = L"";
    _wgetcwd(fp, _MAX_PATH);
    strcpy(path, nall::utf8_t(fp));
    return path;
  }

  intptr_t execv(const char *cmdname, const char *const *argv) {
    int argc = __argc;
    wchar_t **wargv = CommandLineToArgvW(GetCommandLineW(), &argc);
    return _wexecv(nall::utf16_t(cmdname), wargv);
  }
#endif

int main(int argc, char **argv) {
  char path[PATH_MAX], *unused;
  unused = realpath(argv[0], path);
  string realPath = dir(path);
  string basePath = string(dir(path), "bsnes.cfg");
  unused = userpath(path);
  if(!strend(path, "/") && !strend(path, "\\")) strcat(path, "/");
  string userPath = string(path, ".bsnes/bsnes.cfg");

  configuration config;
  string profile;
  config.attach(profile = "", "system.profile");
  if(config.load(userPath) == false) config.load(basePath);
  if(profile == "") profile = "compatibility";

  string binaryName = string("bsnes-", profile);
  #if defined(PLATFORM_WIN)
  binaryName << ".dll";
  #endif

  string fileName = string("/usr/local/bin/", binaryName);
  if(!file::exists(fileName)) fileName = string("/usr/bin/", binaryName);
  if(!file::exists(fileName)) fileName = string(realPath, binaryName);
  if(!file::exists(fileName)) {
    #if defined(PLATFORM_WIN)
    MessageBox(0, string("Error: unable to locate binary file :", binaryName), "bsnes", MB_OK);
    #else
    print("[bsnes] Error: unable to locate binary file: ", binaryName, "\n");
    #endif
    return 0;
  }

  execv(fileName, argv);
  return 0;
}
