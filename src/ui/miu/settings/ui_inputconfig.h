class InputConfigWindow : public Window {
public:
  Label    lportA;
  Label    lportB;
  Combobox portA;
  Combobox portB;
  Listbox  list;
  Button   setkey;
  Button   clrkey;

  void setup();
  void refresh_list();

  uintptr_t list_change(Event);
  uintptr_t set_tick(Event);
  uintptr_t clr_tick(Event);

  static const char list_index[][64];
  uint get_value(uint index);
  void set_value(uint index, uint16 value);
} window_input_config;

class InputCaptureWindow : public Window {
public:
  Label label;

  uint index;
  bool key_lock;

  void setup();
  void show();

  uintptr_t close(Event);

  InputCaptureWindow();
} window_input_capture;
