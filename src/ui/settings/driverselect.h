class DriverSelectWindow : public Window {
public:
  Label    crash_message;

  Label    lvideo;
  Combobox cvideo;
  Label    laudio;
  Combobox caudio;
  Label    linput;
  Combobox cinput;

  Label    video_caps;
  Checkbox video_sync;
  Checkbox video_filter;

  Label    audio_caps;
  Checkbox audio_sync;
  Checkbox audio_freq;
  Checkbox audio_latency;

  Label    input_caps;
  Checkbox input_keyboard;
  Checkbox input_joypad;

  Label    restart_message;

  uintptr_t video_change(event_t);
  uintptr_t audio_change(event_t);
  uintptr_t input_change(event_t);

  void setup();
} window_driver_select;
