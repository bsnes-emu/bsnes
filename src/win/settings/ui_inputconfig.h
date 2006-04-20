class InputConfigWindow : public Window {
public:
Label    Header;
Editbox  ResistanceDesc;
Label    ResistanceLabel;
Slider   Resistance;
Label    Separator;
Label    SelectLabel;
Combobox Selected;
Label    Message;
Button   ButtonL, ButtonR;
Button   ButtonUp, ButtonDown, ButtonLeft, ButtonRight;
Button   ButtonSelect, ButtonStart;
Button   ButtonA, ButtonB, ButtonX, ButtonY;
Checkbox AllowBadInput;

struct {
  bool active;
  uint controller;
  uint button;
  uint id;
} button_update;

enum {
  BUTTON_L, BUTTON_R,
  BUTTON_UP, BUTTON_DOWN, BUTTON_LEFT, BUTTON_RIGHT,
  BUTTON_SELECT, BUTTON_START,
  BUTTON_A, BUTTON_B, BUTTON_X, BUTTON_Y,
};

  bool Event(EventInfo &info);
  void Show();
  void Hide();

  const char *GetCaption(uint button);
  void ButtonUpdate(uint button);
  void ButtonUpdateEnd();
  void Setup();

  InputConfigWindow();
} wInputConfig;
