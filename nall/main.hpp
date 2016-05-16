#pragma once

#include <nall/platform.hpp>
#include <nall/string.hpp>

namespace nall {
  auto main(lstring arguments) -> void;

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

    lstring arguments;
    for(auto n : range(argc)) arguments.append(argv[n]);

    return main(move(arguments)), EXIT_SUCCESS;
  }
}

auto main(int argc, char** argv) -> int {
  return nall::main(argc, argv);
}
