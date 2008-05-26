/*
  hiro
  version: 0.005 (2008-05-25)
  author: byuu
  license: public domain
*/

#ifndef HIRO_H
#define HIRO_H

#include <nall/array.hpp>
#include <nall/function.hpp>
#include <nall/input.hpp>
#include <nall/new.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>
#include <nall/utility.hpp>
typedef unsigned int uint;

namespace libhiro {

class pHiro;
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
  friend class pHiro; \
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

class Hiro;
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

typedef nall::array<MenuRadioItem*> MenuRadioItemGroup;
typedef nall::array<Radiobox*> RadioboxGroup;

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

class Hiro : nall::noncopyable {
public:
  void init();
  void term();
  bool run();
  bool pending();

  bool folder_select(Window *focus, char *filename, const char *path = "");
  bool file_open(Window *focus, char *filename, const char *path = "", const char *filter = "");
  bool file_save(Window *focus, char *filename, const char *path = "", const char *filter = "");

  uint screen_width();
  uint screen_height();

  void enable_screensaver();
  void disable_screensaver();

  static Hiro& handle();
  Hiro();
  ~Hiro();

private:
  pFriends;
  pHiro &p;
};

Hiro& hiro();

class Widget : nall::noncopyable {
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

class Window : private nall::base_from_member<pWindow&>, public Widget {
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
  uint get_width();
  uint get_height();
  void set_opacity(uint8_t opacity);
  void set_background_color(uint8_t r, uint8_t g, uint8_t b);
  void set_icon(unsigned width, unsigned height, const uint32_t *data);
  void set_status_text(const char *text = "");
  void set_text(const char *text = "");
  void attach(Window &window, uint x, uint y);
  void attach(MenuGroup &menugroup);
  void attach(FormControl &formcontrol, uint x, uint y);
  void move(Window &window, uint x, uint y);
  void move(FormControl &formcontrol, uint x, uint y);

  class Menubar {
  public:
    void show(bool = true);
    void hide();
    bool visible();

    pWindow &p;
    Menubar(pWindow&);
  } menu;

  class Statusbar {
  public:
    void set_text(const char *text = "");
    void show(bool = true);
    void hide();
    bool visible();

    pWindow &p;
    Statusbar(pWindow&);
  } status;

  nall::function<uintptr_t (Event)> on_close;
  nall::function<uintptr_t (Event)> on_block;
  nall::function<uintptr_t (Event)> on_keydown;
  nall::function<uintptr_t (Event)> on_keyup;

  Window();

private:
  pFriends;
  pWindow &p;
};

class MenuControl : public nall::base_from_member<pMenuControl&>, public Widget {
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

class MenuGroup : public nall::base_from_member<pMenuGroup&>, public MenuControl {
public:
  MenuGroup& create(const char *text);
  void attach(MenuControl &menucontrol);
  MenuGroup();

private:
  pFriends;
  pMenuGroup &p;
};

class MenuItem : public nall::base_from_member<pMenuItem&>, public MenuControl {
public:
  MenuItem& create(const char *text);
  MenuItem();

  nall::function<uintptr_t (Event)> on_tick;

private:
  pFriends;
  pMenuItem &p;
};

class MenuCheckItem : public nall::base_from_member<pMenuCheckItem&>, public MenuControl {
public:
  MenuCheckItem& create(const char *text);
  void check(bool = true);
  void uncheck();
  bool checked();
  MenuCheckItem();

  nall::function<uintptr_t (Event)> on_tick;

private:
  pFriends;
  pMenuCheckItem &p;
};

class MenuRadioItem : public nall::base_from_member<pMenuRadioItem&>, public MenuControl {
public:
  MenuRadioItem& create(MenuRadioItemGroup &group, const char *text);
  void check();
  bool checked();
  MenuRadioItem();

  nall::function<uintptr_t (Event)> on_tick;

private:
  pFriends;
  pMenuRadioItem &p;
};

class MenuSeparator : public nall::base_from_member<pMenuSeparator&>, public MenuControl {
public:
  MenuSeparator& create();
  MenuSeparator();

private:
  pFriends;
  pMenuSeparator &p;
};

class FormControl : private nall::base_from_member<pFormControl&>, public Widget {
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

class Frame : private nall::base_from_member<pFrame&>, public FormControl {
public:
  void create(uint style, uint width, uint height, const char *text = "");
  void set_text(const char *text = "");

  Frame();

private:
  pFriends;
  pFrame &p;
};

class Canvas : private nall::base_from_member<pCanvas&>, public FormControl {
public:
  void create(uint style, uint width, uint height);
  void redraw();
  uint32_t* buffer();

  Canvas();

private:
  pFriends;
  pCanvas &p;
};

class Label : private nall::base_from_member<pLabel&>, public FormControl {
public:
  void create(uint style, uint width, uint height, const char *text = "");
  void set_text(const char *text = "");

  Label();

private:
  pFriends;
  pLabel &p;
};

class Button : private nall::base_from_member<pButton&>, public FormControl {
public:
  void create(uint style, uint width, uint height, const char *text = "");
  void set_text(const char *text = "");

  nall::function<uintptr_t (Event)> on_tick;

  Button();

private:
  pFriends;
  pButton &p;
};

class Checkbox : private nall::base_from_member<pCheckbox&>, public FormControl {
public:
  void create(uint style, uint width, uint height, const char *text = "");
  void set_text(const char *text = "");
  void check(bool = true);
  void uncheck();
  bool checked();

  nall::function<uintptr_t (Event)> on_tick;

  Checkbox();

private:
  pFriends;
  pCheckbox &p;
};

class Radiobox : private nall::base_from_member<pRadiobox&>, public FormControl {
public:
  void create(RadioboxGroup &group, uint style, uint width, uint height, const char *text = "");
  void set_text(const char *text = "");
  void check();
  bool checked();

  nall::function<uintptr_t (Event)> on_tick;

  Radiobox();

private:
  pFriends;
  pRadiobox &p;
};

class Editbox : private nall::base_from_member<pEditbox&>, public FormControl {
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

class Listbox : private nall::base_from_member<pListbox&>, public FormControl {
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

  nall::function<uintptr_t (Event)> on_change;
  nall::function<uintptr_t (Event)> on_activate;

  Listbox();

private:
  pFriends;
  pListbox &p;
};

class Combobox : private nall::base_from_member<pCombobox&>, public FormControl {
public:
  void create(uint style, uint width, uint height, const char *text = "");
  void add_item(const char *text);
  int  get_selection();
  void set_selection(int index);
  void reset();

  nall::function<uintptr_t (Event)> on_change;

  Combobox();

private:
  pFriends;
  pCombobox &p;
};

class Progressbar : private nall::base_from_member<pProgressbar&>, public FormControl {
public:
  void create(uint style, uint width, uint height);
  uint get_progress();
  void set_progress(uint progress);

  Progressbar();

private:
  pFriends;
  pProgressbar &p;
};

class Slider : private nall::base_from_member<pSlider&>, public FormControl {
public:
  enum Style {
    Horizontal = 0,
    Vertical = 1 << 1,
  };

  void create(uint style, uint width, uint height, uint length);
  uint get_position();
  void set_position(uint position);

  nall::function<uintptr_t (Event)> on_change;

  Slider();

private:
  pFriends;
  pSlider &p;
};

#undef pFriends

} //namespace libhiro

#endif //ifndef HIRO_H
