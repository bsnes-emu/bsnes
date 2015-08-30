#if defined(Hiro_TabFrame)

namespace hiro {

auto pTabFrameItem::construct() -> void {
}

auto pTabFrameItem::destruct() -> void {
}

auto pTabFrameItem::append(sLayout layout) -> void {
  if(auto parent = _parent()) {
    parent->_synchronizeLayout();
  }
}

auto pTabFrameItem::remove(sLayout layout) -> void {
  if(auto parent = _parent()) {
    parent->_synchronizeLayout();
  }
}

auto pTabFrameItem::setClosable(bool closable) -> void {
  //unsupported
}

auto pTabFrameItem::setImage(const Image& image) -> void {
  if(auto parent = _parent()) {
    parent->_buildImageList();
  }
}

auto pTabFrameItem::setMovable(bool movable) -> void {
  //unsupported
}

auto pTabFrameItem::setSelected() -> void {
  if(auto parent = _parent()) {
    TabCtrl_SetCurSel(parent->hwnd, self().offset());
    parent->_synchronizeLayout();
  }
}

auto pTabFrameItem::setText(const string& text) -> void {
  if(auto parent = _parent()) {
    utf16_t wText(text);
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
    tcItem.pszText = (wchar_t*)wText;
    TabCtrl_SetItem(parent->hwnd, self().offset(), &tcItem);
  }
}

auto pTabFrameItem::_parent() -> maybe<pTabFrame&> {
  if(auto parent = self().parentTabFrame()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

}

#endif
