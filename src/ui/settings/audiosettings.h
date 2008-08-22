class AudioSettingsWindow : public Window {
public:
  Label   lvolume;
  Slider  svolume;
  Editbox evolume;
  Button  bvolume;

  Label   llatency;
  Slider  slatency;
  Editbox elatency;
  Button  blatency;

  Label   loutput;
  Slider  soutput;
  Editbox eoutput;
  Button  boutput;

  Label   linput;
  Slider  sinput;
  Editbox einput;
  Button  binput;

  uintptr_t volume_change(event_t);
  uintptr_t latency_change(event_t);
  uintptr_t output_change(event_t);
  uintptr_t input_change(event_t);

  uintptr_t volume_tick(event_t);
  uintptr_t latency_tick(event_t);
  uintptr_t output_tick(event_t);
  uintptr_t input_tick(event_t);

  void sync_ui();
  void setup();
} window_audio_settings;
