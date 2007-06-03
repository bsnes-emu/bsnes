class InputCaptureWindow : public ui::Window { public:
ui::Label label;
uint index;
  int  message(uint id, void *param = 0);
  void setup();
  InputCaptureWindow() : index(0) {}
} window_input_capture;

class InputConfigWindow : public ui::Window { public:
ui::Label    lportA;
ui::Label    lportB;
ui::Combobox portA;
ui::Combobox portB;
ui::Listbox  list;
ui::Button   setkey;
ui::Button   clrkey;

static const char list_index[][64];

  int  message(uint id, void *param = 0);
  uint get_value(uint index);
  void set_value(uint index, uint value);
  void refresh_list();
  void setup();
} window_input_config;
