class Control {
public:
enum {
  LABEL,
  EDITBOX,
  BUTTON,
  CHECKBOX,
  RADIOBOX,
  SLIDER,
  COMBOBOX,
  LISTBOX,
  LISTVIEW,
};

HWND    hwnd;
HBRUSH  backbrush;
Window *parent;
uint    id, type;

struct State {
  uint x, y, width, height;
  uint es, ws;
  bool use_backcolor, use_textcolor;
  uint backcolor, textcolor;
} state;

  virtual bool Create(Window *parent_window, const char *style, int x, int y, int width, int height, const char *text = "") = 0;
  void PostCreate();

  virtual void Show(bool do_show);
  virtual bool Visible();
  virtual void Show();
  virtual void Hide();
  virtual void Focus();

  virtual void Enable(bool do_enable);
  virtual bool Enabled();
  virtual void Enable();
  virtual void Disable();

  uint GetID();
  uint GetType();
  void SetBackgroundColor(uint r, uint g, uint b);
  void SetTextColor(uint r, uint g, uint b);
  void SetFont(Font font);
  const char *GetText();
  void GetText(char *text, uint length = 0);
  void SetText(const char *text, ...);

/* Functions for all controls are defined as empty virtual functions
 * inside the base control class, so that event handler can directly
 * invoke derived control class functions without the need for a
 * static_cast. Not the best programming practice, but nothing bad
 * should result from unintentionally calling an empty function.
 */

//Editbox
  virtual void SetSelection(int begin, int end) {}

//Checkbox, Radiobox
  virtual bool Checked() { return false; }
  virtual void Check(bool state) {}
  virtual void Check() {}
  virtual void Uncheck() {}
  virtual void ToggleCheck() {}

//Slider
  virtual void SetRange(int min, int max) {}
  virtual void SetPos(int pos) {}
  virtual int  GetPos() { return 0; }

//Combobox, Listbox, Listview
  virtual void AddItem(const char *text) {}
  virtual void DeleteItem(uint id) {}
  virtual void DeleteAllItems() {}
  virtual uint GetItemCount() { return 0; }
  virtual void SetSelection(uint id) {}
  virtual int  GetSelection() { return 0; }

//Listview
  virtual void AddColumn(uint align, uint width, const char *text) {}
  virtual void SetItemText(uint id, const char *text) {}

  Control();
  ~Control();
};

class Label : public Control {
public:
  bool Create(Window *parent_window, const char *style, int x, int y, int width, int height, const char *text = "");
};

class Groupbox : public Control {
public:
  bool Create(Window *parent_window, const char *style, int x, int y, int width, int height, const char *text = "");
};

class Editbox : public Control {
public:
  bool Create(Window *parent_window, const char *style, int x, int y, int width, int height, const char *text = "");

  void SetSelection(int begin, int end);
};

class Button : public Control {
public:
  bool Create(Window *parent_window, const char *style, int x, int y, int width, int height, const char *text = "");
};

class Checkbox : public Control {
public:
  bool Create(Window *parent_window, const char *style, int x, int y, int width, int height, const char *text = "");

  bool Checked();
  void Check(bool state);
  void Check();
  void Uncheck();
  void ToggleCheck();
};

class Radiobox : public Control {
public:
  bool Create(Window *parent_window, const char *style, int x, int y, int width, int height, const char *text = "");

  bool Checked();
  void Check(bool state);
  void Check();
  void Uncheck();
  void ToggleCheck();
};

class Slider : public Control {
public:
  bool Create(Window *parent_window, const char *style, int x, int y, int width, int height, const char *text = "");

  void SetRange(int min, int max);
  void SetPos(int pos);
  int  GetPos();
};

class Combobox : public Control {
public:
  bool Create(Window *parent_window, const char *style, int x, int y, int width, int height, const char *text = "");

  void AddItem(const char *text);
  void DeleteItem(uint id);
  void DeleteAllItems();
  uint GetItemCount();

  void SetSelection(uint id);
  int  GetSelection();
};

class Listbox : public Control {
public:
  bool Create(Window *parent_window, const char *style, int x, int y, int width, int height, const char *text = "");

  void AddItem(const char *text);
  void DeleteItem(uint id);
  void DeleteAllItems();
  uint GetItemCount();

  void SetSelection(uint id);
  int  GetSelection();
};

class Listview : public Control {
private:
uint column_count;

public:
  bool Create(Window *parent_window, const char *style, int x, int y, int width, int height, const char *text = "");

enum { LEFT, RIGHT };
enum { NONE = -1 };
  void AddColumn(uint align, uint width, const char *text);
  void AddItem(const char *text);
  void SetItemText(uint id, const char *text);
  void DeleteAllItems();
  uint GetItemCount();

  void SetSelection(uint id);
  int  GetSelection();

  Listview() : column_count(0) {}
};
