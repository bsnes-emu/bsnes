char* realpath(const char *file_name, char *resolved_name) {
  wchar_t filename[_MAX_PATH] = L"";
  _wfullpath(filename, utf16(file_name), _MAX_PATH);
  strcpy(resolved_name, utf8(filename));
  return resolved_name;
}

char* userpath(char *output) {
  wchar_t path[_MAX_PATH] = L"";
  SHGetFolderPathW(0, CSIDL_APPDATA | CSIDL_FLAG_CREATE, 0, 0, path);
  strcpy(output, utf8(path));
  return output;
}

int mkdir(const char *path) {
  return _wmkdir(utf16(path));
}

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  //argv[] is in 7-bit ANSI format; Unicode characters are converted to '?'s.
  //this needs to be converted to UTF-8, eg for realpath(argv[0]) to work.
  int argc;
  wchar_t **wargv = CommandLineToArgvW(GetCommandLineW(), &argc);
  char **argv = new char*[argc];
  for(unsigned i = 0; i < argc; i++) {
    argv[i] = new char[_MAX_PATH];
    strcpy(argv[i], utf8(wargv[i]));
  }

  libhiro::hiro().init();
  int result = hiromain(argc, argv);
  libhiro::hiro().term();

  for(unsigned i = 0; i < argc; i++) {
    delete[] argv[i];
  }
  delete[] argv;

  return result;
}
