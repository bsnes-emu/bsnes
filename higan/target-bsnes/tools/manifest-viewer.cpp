auto ManifestViewer::create() -> void {
  setIcon(Icon::Emblem::Text);
  setText("Manifest Viewer");

  layout.setPadding(5);
  manifestLabel.setText("Manifest:");
  manifestOption.onChange([&] { selectManifest(); });
  manifestSpacer.setColor({192, 192, 192});
  #if 0 && defined(Hiro_SourceEdit)
  manifestView.setFont(Font().setFamily(Font::Mono).setSize(10));
  #else
  manifestView.setFont(Font().setFamily(Font::Mono));
  #endif
  manifestView.setEditable(false);
  manifestView.setWordWrap(false);
  typeIcon.setIcon(Icon::Device::Storage);
  nameLabel.setText("...");
}

auto ManifestViewer::loadManifest() -> void {
  manifestOption.reset();
  manifestView.setText("");
  if(!emulator->loaded()) return;

  auto manifests = emulator->manifests();
  auto titles = emulator->titles();
  for(uint offset : range(manifests.size())) {
    ComboButtonItem item{&manifestOption};
    item.setProperty("manifest", manifests[offset]);
    item.setText(titles[offset]);
    bool verified = false;
    if(offset == 0) verified = program.superFamicom.verified;
    if(offset == 1 && program.gameBoy) verified = program.gameBoy.verified;
    if(offset == 1 && program.bsMemory) verified = program.bsMemory.verified;
    if(offset == 1 && program.sufamiTurboA) verified = program.sufamiTurboA.verified;
    if(offset == 2 && program.sufamiTurboB) verified = program.sufamiTurboB.verified;
    item.setIcon(verified ? Icon::Emblem::Program : Icon::Emblem::Binary);
  }
  manifestOption.doChange();
}

auto ManifestViewer::selectManifest() -> void {
  auto selected = manifestOption.selected();
  uint offset = selected->offset();
  manifestView.setText(selected.property("manifest"));
  string location;
  if(offset == 0) location = program.superFamicom.location;
  if(offset == 1 && program.gameBoy) location = program.gameBoy.location;
  if(offset == 1 && program.bsMemory) location = program.bsMemory.location;
  if(offset == 1 && program.sufamiTurboA) location = program.sufamiTurboA.location;
  if(offset == 2 && program.sufamiTurboB) location = program.sufamiTurboB.location;
  typeIcon.setIcon(location.endsWith("/") ? Icon::Action::Open : Icon::Emblem::File);
  nameLabel.setText(location.trimRight("/", 1L));
}
