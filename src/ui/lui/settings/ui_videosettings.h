class VideoSettingsWindow : public ui::Window { public:
ui::Label    label_select;
ui::Combobox mode_select;

ui::Label    label_region, label_scalar, label_hwfilter, label_swfilter;
ui::Combobox region, scalar, hwfilter, swfilter;

ui::Checkbox aspect, vsync;

ui::Label    label_fsmode;
ui::Editbox  fs_width, fs_height, fs_refresh;
  bool message(uint, uintptr_t);
  void setup();
} window_video_settings;
