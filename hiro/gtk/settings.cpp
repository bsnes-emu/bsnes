namespace hiro {

Settings::Settings() {
  string path = {Path::userData(), "hiro/"};
  #if HIRO_GTK==2
  auto document = BML::unserialize(file::read({path, "gtk2.bml"}));
  #elif HIRO_GTK==3
  auto document = BML::unserialize(file::read({path, "gtk3.bml"}));
  #endif

  auto get = [&](string_view name) {
    return document[name];
  };

  geometry.frameX = get("Geometry/FrameX").integer();
  geometry.frameY = get("Geometry/FrameY").integer();
  geometry.frameWidth = get("Geometry/FrameWidth").integer();
  geometry.frameHeight = get("Geometry/FrameHeight").integer();
  geometry.menuHeight = get("Geometry/MenuHeight").integer();
  geometry.statusHeight = get("Geometry/StatusHeight").integer();
}

Settings::~Settings() {
  string path = {Path::userData(), "hiro/"};
  directory::create(path, 0755);

  Markup::Node document;
  auto set = [&](string_view name, string_view value) {
    document(name).setValue(value);
  };

  set("Geometry/FrameX", geometry.frameX);
  set("Geometry/FrameY", geometry.frameY);
  set("Geometry/FrameWidth", geometry.frameWidth);
  set("Geometry/FrameHeight", geometry.frameHeight);
  set("Geometry/MenuHeight", geometry.menuHeight);
  set("Geometry/StatusHeight", geometry.statusHeight);

  #if HIRO_GTK==2
  file::write({path, "gtk2.bml"}, BML::serialize(document));
  #elif HIRO_GTK==3
  file::write({path, "gtk3.bml"}, BML::serialize(document));
  #endif
}

}
