//platform-specific wrappers

#if defined(_WIN32)
  //Windows 32-bit and 64-bit
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
  #include <nall/utf8.hpp>

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

  int mkdir(const char *path) {
    return _wmkdir(nall::utf16_t(path));
  }

  void initargs(int &argc, char **&argv) {
    wchar_t **wargv = CommandLineToArgvW(GetCommandLineW(), &argc);
    argv = new char*[argc];
    for(unsigned i = 0; i < argc; i++) {
      argv[i] = new char[_MAX_PATH];
      strcpy(argv[i], nall::utf8_t(wargv[i]));
    }
  }
#else
  //POSIX-compatible (Linux, BSD, etc.)
  char* userpath(char *path) {
    *path = 0;
    struct passwd *userinfo = getpwuid(getuid());
    if(userinfo) strcpy(path, userinfo->pw_dir);
    return path;
  }

  char *getcwd(char *path) {
    return getcwd(path, PATH_MAX);
  }

  #define mkdir(path) (mkdir)(path, 0755)

  void initargs(int &argc, char **&argv) {
  }
#endif
