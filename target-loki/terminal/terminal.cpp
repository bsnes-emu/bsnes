#include "../loki.hpp"
Terminal* terminal = nullptr;

Terminal::Terminal() {
  terminal = this;

  setWindowGeometry({0, 480 + frameMargin().height, 800, 480});

  console.setFont(Font::monospace(8));
  print("loki v", Emulator::Version, "\n\n");
  print("$ ");

  layout.append(console, {~0, ~0});
  append(layout);

  onClose = &Application::quit;
  console.onActivate = {&Terminal::command, this};
}

void Terminal::command(string s) {
  if(s.empty()) {
  } else if(s == "quit" || s == "exit") {
    Application::quit();
  } else if(s == "clear" || s == "reset") {
    reset();
  } else if(s == "r") {
    program->pause = false;
  } else if(s == "p") {
    program->pause = true;
  } else {
    print("unrecognized command\n");
  }
  print("$ ");
}

void Terminal::reset() {
  console.reset();
}

template<typename... Args> void Terminal::print(Args&&... args) {
  console.print(std::forward<Args>(args)...);
}
