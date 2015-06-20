#include "tomoko.hpp"
Video* video = nullptr;
Audio* audio = nullptr;
Input* input = nullptr;
Emulator::Interface* emulator = nullptr;

#include <nall/main.hpp>
auto nall::main(lstring args) -> void {
  Application::setName("tomoko");
  new Program;
  Application::run();
}
