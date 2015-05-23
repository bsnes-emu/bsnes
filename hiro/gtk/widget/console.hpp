#if defined(Hiro_Console)

namespace hiro {

struct pConsole : pWidget {
  Declare(Console, Widget)

  auto print(const string& text) -> void;
  auto reset() -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setForegroundColor(Color color) -> void;
  auto setPrompt(const string& prompt) -> void;

  auto _keyPress(unsigned scancode, unsigned mask) -> bool;
  auto _seekToEnd() -> void;
  auto _seekToMark() -> void;

  GtkWidget* subWidget = nullptr;
  GtkTextBuffer* textBuffer = nullptr;
  string previousPrompt;
  lstring history;
  unsigned historyOffset = 0;
};

}

#endif
