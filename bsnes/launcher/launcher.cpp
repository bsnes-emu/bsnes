#include <nall/config.hpp>
#include <nall/detect.hpp>
#include <nall/file.hpp>
#include <nall/foreach.hpp>
#include <nall/platform.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>
using namespace nall;

int main(int argc, char **argv) {
  char path[PATH_MAX], *unused;
  #if !defined(PLATFORM_WIN)
  unused = realpath(argv[0], path);
  #else
  wchar_t **argw = CommandLineToArgvW(GetCommandLineW(), &argc);
  unused = realpath(nall::utf8_t(argw[0]), path);
  #endif
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
  string fileName = string(realPath, binaryName);

  #if !defined(PLATFORM_WIN)
  char **args = new char*[argc + 1];
  args[0] = strdup(binaryName);
  for(unsigned i = 1; i < argc; i++) args[i] = strdup(argv[i]);
  args[argc] = 0;
  execvp(args[0], args);
  execv(fileName, args);
  print("[bsnes] Error: unable to locate binary file: ", binaryName, "\n");
  #else
  STARTUPINFOW si;
  PROCESS_INFORMATION pi;
  memset(&si, 0, sizeof(STARTUPINFOW));
  if(!CreateProcessW(nall::utf16_t(fileName), GetCommandLineW(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
    MessageBoxA(0, string("Error: unable to locate binary file: ", binaryName), "bsnes", MB_OK);
  }
  #endif

  return 0;
}
