#include "loki.hpp"

Program* program = nullptr;
DSP dspaudio;

string Program::path(string name) {
  string path = {basepath, name};
  if(file::exists(path) || directory::exists(path)) return path;
  path = {userpath, name};
  if(file::exists(path) || directory::exists(path)) return path;
  path = {sharedpath, name};
  if(file::exists(path) || directory::exists(path)) return path;
  return {userpath, name};
}

void Program::main() {
}

Program::Program(int argc, char** argv) {
  program = this;

  basepath = nall::programpath();
  userpath = {nall::configpath(), "loki/"};
  sharedpath = {nall::sharedpath(), "loki/"};
  directory::create(userpath);
}

int main(int argc, char** argv) {
  #if defined(PLATFORM_WINDOWS)
  utf8_args(argc, argv);
  #endif

  Application::setName("loki");
  new Program(argc, argv);
  delete program;

  return 0;
}
