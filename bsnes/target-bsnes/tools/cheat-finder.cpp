auto CheatFinder::create() -> void {
  setIcon(Icon::Edit::Find);
  setText("Cheat Finder");

  layout.setPadding(5_sx);
  searchList.setHeadered();
  searchValue.onActivate([&] { eventScan(); });
  searchLabel.setText("Value:");
  searchSize.append(ComboButtonItem().setText("Byte"));
  searchSize.append(ComboButtonItem().setText("Word"));
  searchSize.append(ComboButtonItem().setText("Long"));
  searchMode.append(ComboButtonItem().setText("="));
  searchMode.append(ComboButtonItem().setText("!="));
  searchMode.append(ComboButtonItem().setText(">="));
  searchMode.append(ComboButtonItem().setText("<="));
  searchMode.append(ComboButtonItem().setText(">"));
  searchMode.append(ComboButtonItem().setText("<"));
  searchSpan.append(ComboButtonItem().setText("WRAM"));
  searchSpan.append(ComboButtonItem().setText("All"));
  searchScan.setText("Scan").onActivate([&] { eventScan(); });
  searchClear.setText("Clear").onActivate([&] { eventClear(); });

  refresh();
}

auto CheatFinder::restart() -> void {
  searchValue.setText("");
  searchSize.items().first().setSelected();
  searchMode.items().first().setSelected();
  searchSpan.items().first().setSelected();
  candidates.reset();
  refresh();
}

auto CheatFinder::refresh() -> void {
  searchList.reset();
  searchList.append(TableViewColumn().setText("Address"));
  searchList.append(TableViewColumn().setText("Value").setExpandable());

  for(auto& candidate : candidates) {
    TableViewItem item{&searchList};
    item.append(TableViewCell().setText({"0x", hex(candidate.address, 6L)}));
    if(candidate.size == 0) {
      item.append(TableViewCell().setText({"0x", hex(candidate.data, 2L), " (", candidate.data, ")"}));
    }
    if(candidate.size == 1) {
      item.append(TableViewCell().setText({"0x", hex(candidate.data, 4L), " (", candidate.data, ")"}));
    }
    if(candidate.size == 2) {
      item.append(TableViewCell().setText({"0x", hex(candidate.data, 6L), " (", candidate.data, ")"}));
    }
  }

  for(uint n : range(2)) {
    Application::processEvents();
    searchList.resizeColumns();
  }
}

auto CheatFinder::eventScan() -> void {
  uint32_t size = searchSize.selected().offset();
  uint32_t mode = searchMode.selected().offset();
  uint32_t span = searchSpan.selected().offset();
  uint32_t data = searchValue.text().replace("$", "0x").replace("#", "").integer();
  if(size == 0) data &= 0xff;
  if(size == 1) data &= 0xffff;
  if(size == 2) data &= 0xffffff;

  if(!candidates) {
    for(uint address : range(1 << 24)) {
      if((address & 0x40e000) == 0x000000) continue;  //00-3f,80-bf:0000-1fff (WRAM mirrors)
      if((address & 0x40e000) == 0x002000) continue;  //00-3f,80-bf:2000-3fff (I/O)
      if((address & 0x40e000) == 0x004000) continue;  //00-3f,80-bf:4000-5fff (I/O)
      if(span == 0 && (address < 0x7e0000 || address > 0x7fffff)) continue;

      auto value = read(size, address);
      if(compare(mode, value, data)) {
        candidates.append({address, value, size, mode, span});
        if(candidates.size() >= 4096) break;
      }
    }
  } else {
    vector<CheatCandidate> result;
    for(auto& candidate : candidates) {
      uint address = candidate.address;
      if((address & 0x40e000) == 0x000000) continue;  //00-3f,80-bf:0000-1fff (WRAM mirrors)
      if((address & 0x40e000) == 0x002000) continue;  //00-3f,80-bf:2000-3fff (I/O)
      if((address & 0x40e000) == 0x004000) continue;  //00-3f,80-bf:4000-5fff (I/O)
      if(span == 0 && (address < 0x7e0000 || address > 0x7fffff)) continue;

      auto value = read(size, address);
      if(compare(mode, value, data)) {
        result.append({address, value, size, mode, span});
        if(result.size() >= 4096) break;
      }
    }
    candidates = result;
  }

  refresh();
}

auto CheatFinder::eventClear() -> void {
  candidates.reset();
  refresh();
}

auto CheatFinder::read(uint32_t size, uint32_t address) -> uint32_t {
  uint32_t value = 0;
  if(size >= 0) value |= emulator->read(address + 0) <<  0;
  if(size >= 1) value |= emulator->read(address + 1) <<  8;
  if(size >= 2) value |= emulator->read(address + 2) << 16;
  return value;
}

auto CheatFinder::compare(uint32_t mode, uint32_t x, uint32_t y) -> bool {
  if(mode == 0) return x == y;
  if(mode == 1) return x != y;
  if(mode == 2) return x >= y;
  if(mode == 3) return x <= y;
  if(mode == 4) return x >  y;
  if(mode == 5) return x <  y;
  return false;
}
