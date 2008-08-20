class AudioSettingsWindow : public Window {
public:
  Label  lvolume;
  Slider volume;

  Label  llatency;
  Slider latency;

  Label  loutputfrequency;
  Slider outputfrequency;

  Label  linputfrequency;
  Slider inputfrequency;

  uintptr_t volume_change(event_t);
  uintptr_t latency_change(event_t);
  uintptr_t outputfrequency_change(event_t);
  uintptr_t inputfrequency_change(event_t);

  void sync_ui();
  void setup();
} window_audio_settings;
