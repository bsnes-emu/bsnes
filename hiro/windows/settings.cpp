namespace hiro {

Settings::Settings() {
  string path = {Path::userSettings(), "hiro/"};
  auto document = BML::unserialize(file::read({path, "windows.bml"}));

  document["extendedFrameBounds/popup/x"].value(efbPopup.x);
  document["extendedFrameBounds/popup/y"].value(efbPopup.y);
  document["extendedFrameBounds/popup/width"].value(efbPopup.width);
  document["extendedFrameBounds/popup/height"].value(efbPopup.height);

  document["extendedFrameBounds/fixed/x"].value(efbFixed.x);
  document["extendedFrameBounds/fixed/y"].value(efbFixed.y);
  document["extendedFrameBounds/fixed/width"].value(efbFixed.width);
  document["extendedFrameBounds/fixed/height"].value(efbFixed.height);

  document["extendedFrameBounds/resizable/x"].value(efbResizable.x);
  document["extendedFrameBounds/resizable/y"].value(efbResizable.y);
  document["extendedFrameBounds/resizable/width"].value(efbResizable.width);
  document["extendedFrameBounds/resizable/height"].value(efbResizable.height);
}

Settings::~Settings() {
  string path = {Path::userSettings(), "hiro/"};
  directory::create(path, 0755);
  Markup::Node document;

  document("extendedFrameBounds/popup/x").setValue(efbPopup.x);
  document("extendedFrameBounds/popup/y").setValue(efbPopup.y);
  document("extendedFrameBounds/popup/width").setValue(efbPopup.width);
  document("extendedFrameBounds/popup/height").setValue(efbPopup.height);

  document("extendedFrameBounds/fixed/x").setValue(efbFixed.x);
  document("extendedFrameBounds/fixed/y").setValue(efbFixed.y);
  document("extendedFrameBounds/fixed/width").setValue(efbFixed.width);
  document("extendedFrameBounds/fixed/height").setValue(efbFixed.height);

  document("extendedFrameBounds/resizable/x").setValue(efbResizable.x);
  document("extendedFrameBounds/resizable/y").setValue(efbResizable.y);
  document("extendedFrameBounds/resizable/width").setValue(efbResizable.width);
  document("extendedFrameBounds/resizable/height").setValue(efbResizable.height);

  file::write({path, "windows.bml"}, BML::serialize(document));
}

}
