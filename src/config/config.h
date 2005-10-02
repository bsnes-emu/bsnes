extern Config config_file;

namespace config {

extern struct SNES {
  static class VideoColorAdjust : public Setting {
  public:
    void set(uint32 _data);
    SettingOperators(VideoColorAdjust);
  } video_color_curve, video_color_adjust_mode;

  static Setting mute;
} snes;

};
