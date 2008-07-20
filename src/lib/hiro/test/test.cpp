#include "../hiro.h"
using namespace libhiro;

#include <nall/algorithm.hpp>
using nall::min;
using nall::max;

bool kill_ = false;

uint32_t windowicon[16 * 16] = {
  0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000,
  0xff000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000,
  0xff000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000,
  0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000,
  0xff000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000,
  0xff000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000,
  0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000,
  0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000,
  0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000,
  0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000,
  0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000,
  0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000,
  0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000,
  0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000,
  0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000,
  0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000,
};

class SubWindow : public Window {
public:
  Button button;

  uintptr_t tick(Event) {
    hide();
  }

  void setup() {
    create(0, 595, 80);

    button.create(0, 595, 80, "SubWindow (click to hide)");
    button.on_tick = bind(&SubWindow::tick, this);
    attach(button, 0, 0);
  }
} subwindow;

class MainWindow : public Window {
public:
  MenuGroup menu_file;
    MenuGroup menu_file_disk;
      MenuItem menu_file_disk_load;
      MenuItem menu_file_disk_save;
    MenuSeparator menu_file_separator;
    MenuItem menu_file_exit;
  MenuGroup menu_help;
    MenuCheckItem menu_help_check1, menu_help_check2;
    MenuSeparator menu_help_separator1;
    MenuRadioItem menu_help_radio1, menu_help_radio2, menu_help_radio3;
    MenuSeparator menu_help_separator2;
    MenuItem menu_help_about;
  Label label;
  Editbox editbox, editbox_multi;
  Button button_ok;
  Button button_exit;
  Checkbox check1, check2;
  Radiobox radio1, radio2;
  Progressbar progress;
  Combobox combobox;
  Listbox listbox;
  Slider hslider, vslider;
  Frame frame;
  Canvas canvas;

  uintptr_t change(Event e) {
    printf("change(%d)\n", (uint)e.param);
  }

  uintptr_t activate(Event e) {
    printf("activate(%d)\n", (uint)e.param);
  }

  uintptr_t tick(Event e) {
    printf("tick(%d)\n", e.param);
    if(e.widget == &button_ok) {
      char t[4096];
      editbox.get_text(t, 4096);
      printf("'%s'\n", t);
    }
    if(e.widget == &menu_file_disk_load) {
      char t[4096] = "";
      hiro().file_open(0, t, "", "Source files\t*.cpp,*.h\nAll Files\t*.*");
      printf("'%s'\n", t);
    }
    if(e.widget == &menu_file_disk_save) {
      char t[4096] = "";
      hiro().file_save(0, t, "", "Source files\t*.cpp,*.h\nAll Files\t*.*");
      printf("'%s'\n", t);
    }
  }

  uintptr_t keydown(Event e) {
  static bool fs = false;
    if(e.param == nall::keyboard::f11) {
      fs = !fs;
      fs ? fullscreen() : unfullscreen();
      printf("%d -> %4d, %4d\n", fs, get_width(), get_height());
    } else if(e.param == nall::keyboard::escape) {
      menu.show(!menu.visible());
    }
  }

  uintptr_t close(Event) {
    printf("close()\n");
    return kill_ = true;
  }

  void setup() {
    create(Window::AutoCenter, 605, 320, "hiro test application");
    //set_opacity(224);
    //set_background_color(0, 0, 0);
    set_icon(16, 16, windowicon);

    attach(menu_file.create("File"));
      menu_file.attach(menu_file_disk.create("Disk"));
        menu_file_disk.attach(menu_file_disk_load.create("Load ..."));
        menu_file_disk.attach(menu_file_disk_save.create("Save ..."));
      menu_file.attach(menu_file_separator.create());
      menu_file.attach(menu_file_exit.create("Exit"));
    attach(menu_help.create("Help"));
      menu_help.attach(menu_help_check1.create("Check 1"));
      menu_help.attach(menu_help_check2.create("Check 2"));
      menu_help.attach(menu_help_separator1.create());
    { MenuRadioItemGroup group;
      group.add(&menu_help_radio1);
      group.add(&menu_help_radio2);
      group.add(&menu_help_radio3);
      menu_help.attach(menu_help_radio1.create(group, "Radio 1"));
      menu_help.attach(menu_help_radio2.create(group, "Radio 2"));
      menu_help.attach(menu_help_radio3.create(group, "Radio 3"));
    } menu_help.attach(menu_help_separator2.create());
      menu_help.attach(menu_help_about.create("About ..."));
      menu_help_about.disable();
    label.create(0, 200, 35, "hiro test application\n~ byuu");
    editbox.create(0, 200, 25);
    button_ok.create(0, 100, 30, "Ok");
    button_exit.create(0, 100, 30, "Exit");
    editbox_multi.create(Editbox::Multiline | Editbox::VerticalScrollAlways, 200, 95);
    check1.create(0, 100, 20, "Check 1");
    check2.create(0, 100, 20, "Check 2");
  { RadioboxGroup group;
    group.add(&radio1);
    group.add(&radio2);
    radio1.create(group, 0, 100, 20, "Radio 1");
    radio2.create(group, 0, 100, 20, "Radio 2");
  } progress.create(0, 200, 30);
    progress.set_progress(50);
    combobox.create(0, 200, 30);
    combobox.add_item("Option 1");
    combobox.add_item("Option 2");
    combobox.add_item("Option 3");
    listbox.create(Listbox::Header | Listbox::VerticalScrollAlways, 200, 100, "Name\tValue");
    listbox.add_item("a\ttrue");
    listbox.add_item("b\tfalse");
    hslider.create(Slider::Horizontal, 425, 25, 10);
    vslider.create(Slider::Vertical, 25, 200, 10);
    frame.create(0, 155, 225, "Canvas:");

    canvas.create(0, 135, 195);
    for(uint y = 0; y < 195; y++) {
    uint32_t *p = canvas.buffer() + y * 135;
      for(uint x = 0; x < 135; x++) {
      double dx = 128.0 / 135.0 * double(x);
      double dy = 128.0 / 195.0 * double(y);
      uint32_t c = uint32_t(dx) + uint32_t(dy);
        *p++ = (max(0U, min(c, 255U)) ^ 0xff) << 16;
      }
    }

    status.show();
    status.set_text("Statusbar");

    on_close = bind(&MainWindow::close, this);
    on_keydown = bind(&MainWindow::keydown, this);

    menu_file_disk_load.on_tick =
    menu_file_disk_save.on_tick = bind(&MainWindow::tick, this);
    menu_file_exit.on_tick = on_close;
    menu_help_check1.on_tick = menu_help_check2.on_tick =
    menu_help_radio1.on_tick = menu_help_radio2.on_tick =
    menu_help_radio3.on_tick = bind(&MainWindow::tick, this);
    menu_help_about.on_tick = bind(&MainWindow::tick, this);
    button_ok.on_tick = bind(&MainWindow::tick, this);
    button_exit.on_tick = bind(&MainWindow::close, this);
    check1.on_tick = check2.on_tick =
    radio1.on_tick = radio2.on_tick = bind(&MainWindow::tick, this);
    combobox.on_change = bind(&MainWindow::change, this);
    listbox.on_change = bind(&MainWindow::change, this);
    listbox.on_activate = bind(&MainWindow::activate, this);
    hslider.on_change = bind(&MainWindow::change, this);
    vslider.on_change = bind(&MainWindow::change, this);

    attach(label, 5, 5);
    attach(editbox, 5, 40);
    attach(button_ok, 5, 70);
    attach(button_exit, 105, 70);
    attach(editbox_multi, 210, 5);
    attach(check1, 5, 105);
    attach(check2, 105, 105);
    attach(radio1, 5, 125);
    attach(radio2, 105, 125);
    attach(progress, 5, 145);
    attach(combobox, 5, 175);
    attach(listbox, 210, 105);
    attach(hslider, 5, 205);
    attach(vslider, 415, 5);

    attach(frame, 445, 5);
    attach(canvas, 455, 25);

    attach(subwindow, 5, 235);
  }
} window;

int main() {
  hiro().init();
  hiro().disable_screensaver();

  subwindow.setup();
  window.setup();
  window.show();

  window.check1.check();

  while(kill_ == false) hiro().run();

  hiro().term();
  return 0;
}
