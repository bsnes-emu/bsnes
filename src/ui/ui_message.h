class MessageWindow : public Window {
public:
  Label message;
  Button ok;

  void setup();
  void show(const char *message, const char *title = "Warning");
  uintptr_t close(Event);
} window_message;
