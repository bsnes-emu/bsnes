class InputGroup;

class InputConfigWindow : public Window {
public:
  Label    capture_mode;
  Radiobox capture_always;
  Radiobox capture_focus;
  Radiobox capture_pause;
  Combobox config_type;
  Combobox config_subtype;
  Listbox  list;
  Button   setkey;
  Button   clrkey;

  void setup();

  InputGroup* get_group();
  bool assign(uint16_t code);

  void refresh_subtype();
  void refresh_list();

  uintptr_t capture_change(event_t);
  uintptr_t type_change(event_t);
  uintptr_t subtype_change(event_t);
  uintptr_t list_change(event_t);
  uintptr_t set_tick(event_t);
  uintptr_t clr_tick(event_t);
} window_input_config;

class InputCaptureWindow : public Window {
public:
  Label label;
  Canvas canvas;

  bool waiting;
  bool locked;
  uint index;

  void assign(uint16_t code);
  void show();
  void setup();

  uintptr_t close(event_t);

  InputCaptureWindow();
} window_input_capture;
