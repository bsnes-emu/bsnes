class PathSettingsWindow : public Window {
public:
  Label lrompath;
  Editbox rompath;
  Button romselect;
  Button romdefault;

  Label lsavepath;
  Editbox savepath;
  Button saveselect;
  Button savedefault;

  Label lcheatpath;
  Editbox cheatpath;
  Button cheatselect;
  Button cheatdefault;

  Checkbox autodetect;

  uintptr_t selectpath_rom(Event);
  uintptr_t defaultpath_rom(Event);
  uintptr_t selectpath_save(Event);
  uintptr_t defaultpath_save(Event);
  uintptr_t selectpath_cheat(Event);
  uintptr_t defaultpath_cheat(Event);
  uintptr_t autodetect_tick(Event);
  void setup();
} window_path_settings;
