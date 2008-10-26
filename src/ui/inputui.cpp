struct InputUIGeneral : InputGroup {
  InputObject load;
  InputObject pause;
  InputObject reset;
  InputObject power;
  InputObject quit;
  InputObject speed_decrease;
  InputObject speed_increase;
  InputObject frameskip_decrease;
  InputObject frameskip_increase;
  InputObject toggle_fullscreen;
  InputObject toggle_menubar;
  InputObject toggle_statusbar;

  InputUIGeneral()
  : InputGroup("General"),
    load              (InputObject::Button, "Load cartridge", config::input.gui.load),
    pause             (InputObject::Button, "Pause emulation", config::input.gui.pause),
    reset             (InputObject::Button, "Reset system", config::input.gui.reset),
    power             (InputObject::Button, "Power cycle system", config::input.gui.power),
    quit              (InputObject::Button, "Exit emulator", config::input.gui.quit),
    speed_decrease    (InputObject::Button, "Decrease emulation speed", config::input.gui.speed_decrease),
    speed_increase    (InputObject::Button, "Increase emulation speed", config::input.gui.speed_increase),
    frameskip_decrease(InputObject::Button, "Decrease frameskip rate", config::input.gui.frameskip_decrease),
    frameskip_increase(InputObject::Button, "Increase frameskip rate", config::input.gui.frameskip_increase),
    toggle_fullscreen (InputObject::Button, "Toggle fullscreen mode", config::input.gui.toggle_fullscreen),
    toggle_menubar    (InputObject::Button, "Toggle menubar", config::input.gui.toggle_menubar),
    toggle_statusbar  (InputObject::Button, "Toggle statusbar", config::input.gui.toggle_statusbar)
  {
    attach(load); attach(pause); attach(reset); attach(power); attach(quit);
    attach(speed_decrease); attach(speed_increase);
    attach(frameskip_decrease); attach(frameskip_increase);
    attach(toggle_fullscreen); attach(toggle_menubar); attach(toggle_statusbar);
  }
} inputuigeneral;
