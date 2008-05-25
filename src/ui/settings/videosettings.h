class VideoSettingsWindow : public Window {
public:
  Label  lcontrast;
  Label  lbrightness;
  Label  lgamma;
  Slider contrast;
  Slider brightness;
  Slider gamma;

  Checkbox gamma_ramp;
  Checkbox sepia;
  Checkbox grayscale;
  Checkbox invert;

  Button   preset_optimal;
  Button   preset_standard;

  void setup();
  void sync_ui();

  bool ui_lock;
  uintptr_t contrast_change(Event);
  uintptr_t brightness_change(Event);
  uintptr_t gamma_change(Event);
  uintptr_t gammaramp_tick(Event);
  uintptr_t sepia_tick(Event);
  uintptr_t grayscale_tick(Event);
  uintptr_t invert_tick(Event);
  uintptr_t optimal_tick(Event);
  uintptr_t standard_tick(Event);

  VideoSettingsWindow();
} window_video_settings;
