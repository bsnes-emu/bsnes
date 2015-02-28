#include "tomoko.hpp"

#include <nall/main.hpp>
auto nall::main(lstring args) -> void {
  Application::setName("tomoko");
  new Program;
  Application::run();
}
