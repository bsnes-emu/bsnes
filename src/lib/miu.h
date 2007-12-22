/*
  miu
  version: 0.012 (2007-12-22)
  author: byuu
  license: public domain
*/

#ifndef MIU_H
#define MIU_H

#include "bbase.h"
#include "barray.h"
#include "bfunction.h"
#include "bkeymap.h"
#include "bstring.h"

namespace ns_miu {

class pMiu;
class pWidget;
  class pWindow;
  class pMenuControl;
    class pMenuGroup;
    class pMenuItem;
    class pMenuCheckItem;
    class pMenuRadioItem;
    class pMenuSeparator;
  class pFormControl;
    class pFrame;
    class pCanvas;
    class pLabel;
    class pButton;
    class pCheckbox;
    class pRadiobox;
    class pEditbox;
    class pListbox;
    class pCombobox;
    class pProgressbar;
    class pSlider;

#define pFriends \
  friend class pMiu; \
  friend class pWidget; \
    friend class pWindow; \
    friend class pMenuControl; \
      friend class pMenuGroup; \
      friend class pMenuItem; \
      friend class pMenuCheckItem; \
      friend class pMenuRadioItem; \
      friend class pMenuSeparator; \
    friend class pFormControl; \
      friend class pFrame; \
      friend class pCanvas; \
      friend class pLabel; \
      friend class pButton; \
      friend class pCheckbox; \
      friend class pRadiobox; \
      friend class pEditbox; \
      friend class pListbox; \
      friend class pCombobox; \
      friend class pProgressbar; \
      friend class pSlider

class Miu;
class Widget;
  class Window;
  class MenuControl;
    class MenuGroup;
    class MenuItem;
    class MenuCheckItem;
    class MenuRadioItem;
    class MenuSeparator;
  class FormControl;
    class Frame;
    class Canvas;
    class Label;
    class Button;
    class Checkbox;
    class Radiobox;
    class Editbox;
    class Listbox;
    class Combobox;
    class Progressbar;
    class Slider;

typedef array<MenuRadioItem*> MenuRadioItemGroup;
typedef array<Radiobox*> RadioboxGroup;

struct Event {
  enum Type {
    Close,
    Block,
    KeyDown,
    KeyUp,
    Change,
    Tick,
    Activate,
  } type;
  uintptr_t param;
  Widget *widget;

  Event(Type type_, uintptr_t param_ = 0, Widget *widget_ = 0) :
    type(type_), param(param_), widget(widget_) {}
};

class Miu : noncopyable {
public:
  void init();
  void term();
  bool run();
  bool pending();

  bool file_load(Window *focus, char *filename, const char *filter, const char *path);
  bool file_save(Window *focus, char *filename, const char *filter, const char *path);

  uint screen_width();
  uint screen_height();

  static Miu& handle();
  Miu();
  ~Miu();

private:
  pFriends;
  pMiu &p;
};

Miu& miu();

class Widget : noncopyable {
public:
  enum Type {
    WidgetType,
      WindowType,
      MenuControlType,
        MenuGroupType,
        MenuItemType,
        MenuCheckItemType,
        MenuRadioItemType,
        MenuSeparatorType,
      FormControlType,
        FrameType,
        CanvasType,
        LabelType,
        ButtonType,
        CheckboxType,
        RadioboxType,
        EditboxType,
        ListboxType,
        ComboboxType,
        ProgressbarType,
        SliderType,
  } type;

  void show(bool = true);
  void hide();
  bool visible();

  uintptr_t handle();

  Widget();
  ~Widget();

protected:
  Widget(pWidget&);

private:
  pFriends;
  pWidget &p;
};

class Window : private base_from_member<pWindow&>, public Widget {
public:
  enum Style {
    AutoCenter = 1 << 1,
  };

  void create(uint style, uint width, uint height, const char *text = "");
  void close();
  void move(uint x, uint y);
  void resize(uint width, uint height);
  void focus();
  bool focused();
  void fullscreen();
  void unfullscreen();
  void set_background_color(uint8 r, uint8 g, uint8 b);
  void set_text(const char *text = "");
  void attach(Window &window, uint x, uint y);
  void attach(MenuGroup &menugroup);
  void attach(FormControl &formcontrol, uint x, uint y);
  void move(Window &window, uint x, uint y);
  void move(FormControl &formcontrol, uint x, uint y);
  void menu_show(bool = true);
  void menu_hide();
  bool menu_visible();

  function<uintptr_t (Event)> on_close;
  function<uintptr_t (Event)> on_block;
  function<uintptr_t (Event)> on_keydown;
  function<uintptr_t (Event)> on_keyup;

  Window();

protected:
  Window(pWindow&);

private:
  pFriends;
  pWindow &p;
};

class MenuControl : public base_from_member<pMenuControl&>, public Widget {
public:
  void enable(bool = true);
  void disable();
  bool enabled();

  MenuControl();

protected:
  MenuControl(pMenuControl&);

private:
  pFriends;
  pMenuControl &p;
};

class MenuGroup : public base_from_member<pMenuGroup&>, public MenuControl {
public:
  MenuGroup& create(const char *text);
  void attach(MenuControl &menucontrol);
  MenuGroup();

private:
  pFriends;
  pMenuGroup &p;
};

class MenuItem : public base_from_member<pMenuItem&>, public MenuControl {
public:
  MenuItem& create(const char *text);
  MenuItem();

  function<uintptr_t (Event)> on_tick;

private:
  pFriends;
  pMenuItem &p;
};

class MenuCheckItem : public base_from_member<pMenuCheckItem&>, public MenuControl {
public:
  MenuCheckItem& create(const char *text);
  void check(bool = true);
  void uncheck();
  bool checked();
  MenuCheckItem();

  function<uintptr_t (Event)> on_tick;

private:
  pFriends;
  pMenuCheckItem &p;
};

class MenuRadioItem : public base_from_member<pMenuRadioItem&>, public MenuControl {
public:
  MenuRadioItem& create(MenuRadioItemGroup &group, const char *text);
  void check();
  bool checked();
  MenuRadioItem();

  function<uintptr_t (Event)> on_tick;

private:
  pFriends;
  pMenuRadioItem &p;
};

class MenuSeparator : public base_from_member<pMenuSeparator&>, public MenuControl {
public:
  MenuSeparator& create();
  MenuSeparator();

private:
  pFriends;
  pMenuSeparator &p;
};

class FormControl : private base_from_member<pFormControl&>, public Widget {
public:
  void resize(uint width, uint height);
  void focus();
  bool focused();
  void enable(bool = true);
  void disable();
  bool enabled();

  FormControl();

protected:
  FormControl(pFormControl&);

private:
  pFriends;
  pFormControl &p;
};

class Frame : private base_from_member<pFrame&>, public FormControl {
public:
  void create(uint style, uint width, uint height, const char *text = "");
  void set_text(const char *text = "");

  Frame();

private:
  pFriends;
  pFrame &p;
};

class Canvas : private base_from_member<pCanvas&>, public FormControl {
public:
  void create(uint style, uint width, uint height);
  void redraw();
  uint32* buffer();

  Canvas();

private:
  pFriends;
  pCanvas &p;
};

class Label : private base_from_member<pLabel&>, public FormControl {
public:
  void create(uint style, uint width, uint height, const char *text = "");
  void set_text(const char *text = "");

  Label();

private:
  pFriends;
  pLabel &p;
};

class Button : private base_from_member<pButton&>, public FormControl {
public:
  void create(uint style, uint width, uint height, const char *text = "");
  void set_text(const char *text = "");

  function<uintptr_t (Event)> on_tick;

  Button();

private:
  pFriends;
  pButton &p;
};

class Checkbox : private base_from_member<pCheckbox&>, public FormControl {
public:
  void create(uint style, uint width, uint height, const char *text = "");
  void set_text(const char *text = "");
  void check(bool = true);
  void uncheck();
  bool checked();

  function<uintptr_t (Event)> on_tick;

  Checkbox();

private:
  pFriends;
  pCheckbox &p;
};

class Radiobox : private base_from_member<pRadiobox&>, public FormControl {
public:
  void create(RadioboxGroup &group, uint style, uint width, uint height, const char *text = "");
  void set_text(const char *text = "");
  void check();
  bool checked();

  function<uintptr_t (Event)> on_tick;

  Radiobox();

private:
  pFriends;
  pRadiobox &p;
};

class Editbox : private base_from_member<pEditbox&>, public FormControl {
public:
  enum Style {
    Multiline = 1 << 1,
    Readonly  = 1 << 2,

    HorizontalScrollAuto   = 0,
    HorizontalScrollAlways = 1 << 3,
    HorizontalScrollNever  = 1 << 4,

    VerticalScrollAuto   = 0,
    VerticalScrollAlways = 1 << 5,
    VerticalScrollNever  = 1 << 6,
  };

  void create(uint style, uint width, uint height, const char *text = "");
  uint get_text(char *text, uint length = -1U);
  void set_text(const char *text = "");

  Editbox();

private:
  pFriends;
  pEditbox &p;
};

class Listbox : private base_from_member<pListbox&>, public FormControl {
public:
  enum Style {
    Header = 1 << 1,

    HorizontalScrollAuto   = 0,
    HorizontalScrollAlways = 1 << 2,
    HorizontalScrollNever  = 1 << 3,

    VerticalScrollAuto   = 0,
    VerticalScrollAlways = 1 << 4,
    VerticalScrollNever  = 1 << 5,
  };

  void create(uint style, uint width, uint height, const char *columns = "", const char *text = "");
  void autosize_columns();
  void set_column_width(uint column, uint width);
  void add_item(const char *text);
  void set_item(uint index, const char *text);
  int  get_selection();
  void set_selection(int index);
  void reset();

  function<uintptr_t (Event)> on_change;
  function<uintptr_t (Event)> on_activate;

  Listbox();

private:
  pFriends;
  pListbox &p;
};

class Combobox : private base_from_member<pCombobox&>, public FormControl {
public:
  void create(uint style, uint width, uint height, const char *text = "");
  void add_item(const char *text);
  int  get_selection();
  void set_selection(int index);
  void reset();

  function<uintptr_t (Event)> on_change;

  Combobox();

private:
  pFriends;
  pCombobox &p;
};

class Progressbar : private base_from_member<pProgressbar&>, public FormControl {
public:
  void create(uint style, uint width, uint height);
  uint get_progress();
  void set_progress(uint progress);

  Progressbar();

private:
  pFriends;
  pProgressbar &p;
};

class Slider : private base_from_member<pSlider&>, public FormControl {
public:
  enum Style {
    Horizontal = 0,
    Vertical = 1 << 1,
  };

  void create(uint style, uint width, uint height, uint length);
  uint get_position();
  void set_position(uint position);

  function<uintptr_t (Event)> on_change;

  Slider();

private:
  pFriends;
  pSlider &p;
};

#undef pFriends

} //namespace ns_miu

#endif //ifndef MIU_H
