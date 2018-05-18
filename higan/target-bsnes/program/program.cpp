#include "../bsnes.hpp"
#include <sfc/interface/interface.hpp>
#include <gb/interface/interface.hpp>
unique_pointer<Program> program;

Program::Program(string_vector args) {
  program = this;

  Emulator::platform = this;

  new Presentation;
  presentation->setVisible();

  new AboutWindow;

  Application::onMain({&Program::main, this});
}

auto Program::main() -> void {
}

auto Program::quit() -> void {
  Application::quit();
}
