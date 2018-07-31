auto ManifestViewer::create() -> void {
  setIcon(Icon::Emblem::Text);
  setText("Manifest Viewer");

  layout.setPadding(5);
  manifestView.setEditable(false).setWordWrap(false).setFont(Font().setFamily(Font::Mono));
}

auto ManifestViewer::loadManifest() -> void {
  if(!emulator->loaded()) {
    manifestView.setText("");
    verifiedIcon.setIcon({});
    verifiedLabel.setText("");
    return;
  }

  manifestView.setText(emulator->manifests().merge("\n"));
  verifiedIcon.setIcon(program.verified() ? Icon::Emblem::Program : Icon::Emblem::Binary);
  verifiedLabel.setText(program.verified() ? "Verified" : "Unverified");
}
