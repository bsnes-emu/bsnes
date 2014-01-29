#include "loki.hpp"
#include "resource/resource.cpp"

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
  if(pause) {
    usleep(20 * 1000);
    return;
  }

  emulator->run();
}

Program::Program(string pathname) {
  program = this;

  basepath = nall::programpath();
  userpath = {nall::configpath(), "loki/"};
  sharedpath = {nall::sharedpath(), "loki/"};
  directory::create(userpath);

  new Interface;
  new Presentation;
  new Terminal;

  presentation->setVisible();
  terminal->setVisible();
  Application::processEvents();

  video.driver(video.optimalDriver());
  video.set(Video::Handle, presentation->viewport.handle());
  video.set(Video::Synchronize, false);
  video.set(Video::Filter, Video::FilterNearest);
  if(video.init() == false) { video.driver("None"); video.init(); }

  audio.driver(audio.optimalDriver());
  audio.set(Audio::Handle, presentation->viewport.handle());
  audio.set(Audio::Synchronize, true);
  audio.set(Audio::Frequency, 48000u);
  if(audio.init() == false) { audio.driver("None"); audio.init(); }

  input.driver(input.optimalDriver());
  input.set(Input::Handle, presentation->viewport.handle());
  if(input.init() == false) { input.driver("None"); input.init(); }
  input.onChange = {&Interface::inputEvent, interface};

  dspaudio.setPrecision(16);
  dspaudio.setBalance(0.0);
  dspaudio.setFrequency(32000);
  dspaudio.setResampler(DSP::ResampleEngine::Hermite);
  dspaudio.setResamplerFrequency(48000);

  presentation->showSplash();

  interface->load(pathname);

  Application::main = {&Program::main, this};
  Application::run();

  interface->unload();
}

int main(int argc, char** argv) {
  #if defined(PLATFORM_WINDOWS)
  utf8_args(argc, argv);
  #endif

  if(argc != 2 || !directory::exists(argv[1])) {
    print("loki v", Emulator::Version, "\n");
    print("usage: loki /path/game.sfc/\n");
    return 0;
  }

  Application::setName("loki");
  new Program(argv[1]);
  delete program;

  return 0;
}
