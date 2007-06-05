class RasterSettingsWindow : public ui::Window { public:
ui::Label  lcontrast;
ui::Label  lbrightness;
ui::Label  lgamma;
ui::Slider contrast;
ui::Slider brightness;
ui::Slider gamma;

ui::Checkbox gamma_ramp;
ui::Checkbox sepia;
ui::Checkbox grayscale;
ui::Checkbox invert;

ui::Button   preset_optimal;
ui::Button   preset_standard;

  bool message(uint id, uintptr_t param);
  void setup();
  void sync_ui();
} window_raster_settings;
