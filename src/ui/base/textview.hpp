class TextViewWindow : public Window {
public:
  Editbox text;
  Button ok;

  void setup();
  uintptr_t close(event_t);
} window_textview;
