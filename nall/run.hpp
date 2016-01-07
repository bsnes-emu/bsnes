#pragma once

//auto execute(const string& name, const string& args...) -> string;
//[[synchronous]]
//executes program, waits for completion, and returns data written to stdout

//auto invoke(const string& name, const string& args...) -> void;
//[[asynchronous]]
//if a program is specified, it is executed with the arguments provided
//if a file is specified, the file is opened using the program associated with said file type
//if a folder is specified, the folder is opened using the associated file explorer
//if a URL is specified, the default web browser is opened and pointed at the URL requested

#include <nall/intrinsics.hpp>
#include <nall/string.hpp>

namespace nall {

#if defined(PLATFORM_LINUX) || defined(PLATFORM_BSD)

template<typename... P> inline auto execute(const string& name, P&&... p) -> string {
  int fd[2];
  if(pipe(fd) == -1) return "";

  pid_t pid = fork();
  if(pid == 0) {
    const char* argv[1 + sizeof...(p) + 1];
    const char** argp = argv;
    lstring argl(forward<P>(p)...);
    *argp++ = (const char*)name;
    for(auto& arg : argl) *argp++ = (const char*)arg;
    *argp++ = nullptr;

    dup2(fd[1], STDOUT_FILENO);
    dup2(fd[1], STDERR_FILENO);
    close(fd[0]);
    close(fd[1]);
    execvp(name, (char* const*)argv);
    exit(0);
  } else {
    close(fd[1]);

    string result;
    while(true) {
      char buffer[256];
      auto size = read(fd[0], buffer, sizeof(buffer));
      if(size <= 0) break;

      auto offset = result.size();
      result.resize(offset + size);
      memory::copy(result.get() + offset, buffer, size);
    }

    close(fd[0]);
    wait(nullptr);
    return result;
  }
}

template<typename... P> inline auto invoke(const string& name, P&&... p) -> void {
  pid_t pid = fork();
  if(pid == 0) {
    const char* argv[1 + sizeof...(p) + 1];
    const char** argp = argv;
    lstring argl(forward<P>(p)...);
    *argp++ = (const char*)name;
    for(auto& arg : argl) *argp++ = (const char*)arg;
    *argp++ = nullptr;

    if(execvp(name, (char* const*)argv) < 0) {
      execlp("xdg-open", "xdg-open", (const char*)name, nullptr);
    }
    exit(0);
  }
}

#elif defined(PLATFORM_WINDOWS)

template<typename... P> inline auto execute(const string& name, P&&... p) -> string {
  lstring argl(name, forward<P>(p)...);
  for(auto& arg : argl) if(arg.find(" ")) arg = {"\"", arg, "\""};
  string arguments = argl.merge(" ");

  SECURITY_ATTRIBUTES sa;
  ZeroMemory(&sa, sizeof(SECURITY_ATTRIBUTES));
  sa.nLength = sizeof(SECURITY_ATTRIBUTES);
  sa.bInheritHandle = true;
  sa.lpSecurityDescriptor = nullptr;

  HANDLE stdoutRead;
  HANDLE stdoutWrite;
  if(!CreatePipe(&stdoutRead, &stdoutWrite, &sa, 0)) return "";
  if(!SetHandleInformation(stdoutRead, HANDLE_FLAG_INHERIT, 0)) return "";

  HANDLE stdinRead;
  HANDLE stdinWrite;
  if(!CreatePipe(&stdinRead, &stdinWrite, &sa, 0)) return "";
  if(!SetHandleInformation(stdinWrite, HANDLE_FLAG_INHERIT, 0)) return "";

  STARTUPINFO si;
  ZeroMemory(&si, sizeof(STARTUPINFO));
  si.cb = sizeof(STARTUPINFO);
  si.hStdError = stdoutWrite;
  si.hStdOutput = stdoutWrite;
  si.hStdInput = stdinRead;
  si.dwFlags = STARTF_USESTDHANDLES;

  PROCESS_INFORMATION pi;
  ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

  if(!CreateProcess(
    nullptr, utf16_t(arguments),
    nullptr, nullptr, true, CREATE_NO_WINDOW,
    nullptr, nullptr, &si, &pi
  )) return "";

  while(true) {
    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    Sleep(1);
    if(exitCode != STILL_ACTIVE) break;
  }

  string result;
  while(true) {
    char buffer[256];
    DWORD read, available, remaining;
    if(!PeekNamedPipe(stdoutRead, nullptr, sizeof(buffer), &read, &available, &remaining)) break;
    if(read == 0) break;

    if(!ReadFile(stdoutRead, buffer, sizeof(buffer), &read, nullptr)) break;
    if(read == 0) break;

    auto offset = result.size();
    result.resize(offset + read);
    memory::copy(result.get() + offset, buffer, read);
  }

  return result;
}

template<typename... P> inline auto invoke(const string& name, P&&... p) -> void {
  lstring argl(forward<P>(p)...);
  for(auto& arg : argl) if(arg.find(" ")) arg = {"\"", arg, "\""};
  string arguments = argl.merge(" ");
  ShellExecute(nullptr, nullptr, utf16_t(name), utf16_t(arguments), nullptr, SW_SHOWNORMAL);
}

#else

template<typename... P> inline auto execute(const string& name, P&&... p) -> string {
  return "";
}

template<typename... P> inline auto invoke(const string& name, P&&... p) -> void {
}

#endif

}
