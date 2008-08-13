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
  uintptr_t contrast_change(event_t);
  uintptr_t brightness_change(event_t);
  uintptr_t gamma_change(event_t);
  uintptr_t gammaramp_tick(event_t);
  uintptr_t sepia_tick(event_t);
  uintptr_t grayscale_tick(event_t);
  uintptr_t invert_tick(event_t);
  uintptr_t optimal_tick(event_t);
  uintptr_t standard_tick(event_t);

  VideoSettingsWindow();
} window_video_settings;
