class AudioSettingsWindow : public Window {
public:
  Label    lvolume;
  Combobox cvolume;
  Label    lfrequency;
  Combobox cfrequency;
  Label    llatency;
  Combobox clatency;
  Label    lskew;
  Slider   sskew;
  Label    note;

  uintptr_t volume_change(event_t);
  uintptr_t latency_change(event_t);
  uintptr_t frequency_change(event_t);
  uintptr_t skew_change(event_t);

  void sync_ui();
  void setup();
} window_audio_settings;
