#if defined(Hiro_Frame)

namespace hiro {

auto pFrame::construct() -> void {
  qtWidget = qtFrame = new QGroupBox;
  if(QApplication::style()->objectName() == "gtk+") {
    //QGtkStyle (gtk+) theme disrespects font weight and omits the border, even if native GTK+ theme does not
    //bold Label controls already exist; so this style sheet forces QGtkStyle to look like a Frame instead
    qtFrame->setStyleSheet(
      "QGroupBox { border: 1px solid #aaa; border-radius: 5px; margin-top: 0.5em; }\n"
      "QGroupBox::title { left: 5px; subcontrol-origin: margin; }\n"
    );
  }

  pWidget::construct();
  _setState();
}

auto pFrame::destruct() -> void {
  delete qtFrame;
  qtWidget = qtFrame = nullptr;
}

auto pFrame::append(sSizable sizable) -> void {
}

auto pFrame::remove(sSizable sizable) -> void {
}

auto pFrame::setEnabled(bool enabled) -> void {
  if(auto& sizable = state().sizable) sizable->setEnabled(sizable->enabled());
  pWidget::setEnabled(enabled);
}

auto pFrame::setGeometry(Geometry geometry) -> void {
  pWidget::setGeometry(geometry);
  if(auto& sizable = state().sizable) {
    auto size = pFont::size(qtFrame->font(), state().text);
    if(!state().text) size.setHeight(8);
    sizable->setGeometry({
      4, size.height(),
      geometry.width() - 8,
      geometry.height() - size.height() - 4
    });
  }
}

auto pFrame::setText(const string& text) -> void {
  _setState();
}

auto pFrame::setVisible(bool visible) -> void {
  if(auto& sizable = state().sizable) sizable->setVisible(sizable->visible());
  pWidget::setVisible(visible);
}

auto pFrame::_setState() -> void {
  qtFrame->setTitle(QString::fromUtf8(state().text));
}

}

#endif
