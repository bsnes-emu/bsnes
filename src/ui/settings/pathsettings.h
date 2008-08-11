class PathSettingsWindow : public Window {
public:
  Label lrompath;
  Editbox rompath;
  Button romselect;
  Button romdefault;

  Label lpatchpath;
  Editbox patchpath;
  Button patchselect;
  Button patchdefault;

  Label lsavepath;
  Editbox savepath;
  Button saveselect;
  Button savedefault;

  Label lcheatpath;
  Editbox cheatpath;
  Button cheatselect;
  Button cheatdefault;

  uintptr_t selectpath_rom(Event);
  uintptr_t defaultpath_rom(Event);

  uintptr_t selectpath_patch(Event);
  uintptr_t defaultpath_patch(Event);

  uintptr_t selectpath_save(Event);
  uintptr_t defaultpath_save(Event);

  uintptr_t selectpath_cheat(Event);
  uintptr_t defaultpath_cheat(Event);

  void setup();
} window_path_settings;
