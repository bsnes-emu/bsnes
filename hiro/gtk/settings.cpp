namespace hiro {

Settings::Settings() {
  string path = {Path::userSettings(), "hiro/"};
  #if HIRO_GTK==2
  auto document = BML::unserialize(file::read({path, "gtk2.bml"}));
  #elif HIRO_GTK==3
  auto document = BML::unserialize(file::read({path, "gtk3.bml"}));
  #endif

  #define get(name, type, value) \
    if(auto node = document[name]) value = node.type()

  get("Geometry/FrameX", integer, geometry.frameX);
  get("Geometry/FrameY", integer, geometry.frameY);
  get("Geometry/FrameWidth", integer, geometry.frameWidth);
  get("Geometry/FrameHeight", integer, geometry.frameHeight);
  get("Geometry/MenuHeight", integer, geometry.menuHeight);
  get("Geometry/StatusHeight", integer, geometry.statusHeight);

  get("Theme/ActionIcons", boolean, theme.actionIcons);
  get("Theme/WidgetColors", boolean, theme.widgetColors);

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

  set("Theme/ActionIcons", theme.actionIcons);
  set("Theme/WidgetColors", theme.widgetColors);

  #undef set

  #if HIRO_GTK==2
  file::write({path, "gtk2.bml"}, BML::serialize(document));
  #elif HIRO_GTK==3
  file::write({path, "gtk3.bml"}, BML::serialize(document));
  #endif
}

}
