ManifestViewer::ManifestViewer(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Emblem::Text);
  setText("Manifest Viewer");

  layout.setMargin(5);
  manifestView.setEditable(false).setWordWrap(false).setFont(Font().setFamily(Font::Mono));
}

auto ManifestViewer::loadManifest() -> void {
  if(!emulator->loaded()) {
    manifestView.setText("");
    verifiedIcon.setIcon({});
    verifiedLabel.setText("");
    return;
  }

  manifestView.setText(emulator->manifest());
  verifiedIcon.setIcon(program->verified() ? Icon::Emblem::Program : Icon::Emblem::Binary);
  verifiedLabel.setText(program->verified() ? "Verified" : "Unverified");
}
