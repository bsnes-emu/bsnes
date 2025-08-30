namespace hiro {

Settings::Settings() {
  string path = {Path::userSettings(), "hiro/"};
  auto document = BML::unserialize(file::read({path, "qt5.bml"}));

  #define get(name, type, value) \
    if(auto node = document[name]) value = node.type()

  get("Geometry/FrameX", integer, geometry.frameX);
  get("Geometry/FrameY", integer, geometry.frameY);
  get("Geometry/FrameWidth", integer, geometry.frameWidth);
  get("Geometry/FrameHeight", integer, geometry.frameHeight);
  get("Geometry/MenuHeight", integer, geometry.menuHeight);
  get("Geometry/StatusHeight", integer, geometry.statusHeight);

  #undef get
}

Settings::~Settings() {
  string path = {Path::userSettings(), "hiro/"};
  directory::create(path, 0755);

  Markup::Node document;

  #define set(name, value) \
    document(name).setValue(value)

  set("Geometry/FrameX", geometry.frameX);
  set("Geometry/FrameY", geometry.frameY);
  set("Geometry/FrameWidth", geometry.frameWidth);
  set("Geometry/FrameHeight", geometry.frameHeight);
  set("Geometry/MenuHeight", geometry.menuHeight);
  set("Geometry/StatusHeight", geometry.statusHeight);

  #undef set

  file::write({path, "qt5.bml"}, BML::serialize(document));
}

}
