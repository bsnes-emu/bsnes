#pragma once

#include <nall/platform.hpp>
#include <nall/string.hpp>

namespace nall {
  auto main(string_vector arguments) -> void;

  auto main(int argc, char** argv) -> int {
    #if defined(PLATFORM_WINDOWS)
    CoInitialize(0);
    WSAData wsaData{0};
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    _setmode(_fileno(stdin), O_BINARY);
    _setmode(_fileno(stdout), O_BINARY);
    _setmode(_fileno(stderr), O_BINARY);
    utf8_args(argc, argv);
    #endif

    string_vector arguments;
    for(auto n : range(argc)) {
      string argument = argv[n];

      //normalize directory and file path arguments
      if(directory::exists(argument)) argument.transform("\\", "/").trimRight("/").append("/");
      else if(file::exists(argument)) argument.transform("\\", "/").trimRight("/");

      arguments.append(argument);
    }

    return main(move(arguments)), EXIT_SUCCESS;
  }
}

auto main(int argc, char** argv) -> int {
  return nall::main(argc, argv);
}
