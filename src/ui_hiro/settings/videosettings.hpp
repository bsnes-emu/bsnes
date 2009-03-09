class VideoSettingsWindow : public Window {
public:
  Label  lcontrast;
  Label  lbrightness;
  Label  lgamma;
  Slider contrast;
  Slider brightness;
  Slider gamma;

  Checkbox gamma_ramp;
  Checkbox merge_fields;

  void setup();
  void sync_ui();

  uintptr_t contrast_change(event_t);
  uintptr_t brightness_change(event_t);
  uintptr_t gamma_change(event_t);

  uintptr_t gammaramp_tick(event_t);
  uintptr_t merge_fields_tick(event_t);
} window_video_settings;
