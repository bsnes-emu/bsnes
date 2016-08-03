namespace hiro {

Settings::Settings() {
  string path = {Path::local(), "hiro/"};
  auto document = BML::unserialize(file::read({path, "gtk.bml"}));

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
  string path = {Path::local(), "hiro/"};
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

  file::write({path, "gtk.bml"}, BML::serialize(document));
}

}
