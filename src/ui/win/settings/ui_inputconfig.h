class InputConfigWindow : public Window {
public:
Label    Header;
Label    ResistanceLabel;
Slider   Resistance;
Label    SelectLabel;
Combobox Selected;
Label    Separator;
Label    Message;
Listview ButtonList;
Button   ButtonUpdatePrimary;
Button   ButtonUpdateSecondary;
Button   ButtonClear;
Checkbox AllowBadInput;

struct {
  bool primary;
  bool active;
  uint controller;
  uint button;
  uint id;
} button_update;

  uint SelectionToDevice(uint selection);
  uint DeviceToSelection(uint device);

  bool Event(EventInfo &info);
  void Show();
  void Hide();

  const char *GetCaption(uint button);
  void UpdateButtonList();
  void ButtonUpdateBegin(uint button);
  void ButtonUpdateEnd();
  void Setup();

  InputConfigWindow();
} wInputConfig;
