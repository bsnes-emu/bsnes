#if defined(Hiro_HexEdit)

namespace hiro {

auto pHexEdit::construct() -> void {
  qtWidget = qtHexEdit = new QtHexEdit(*this);

  qtHexEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  qtHexEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  qtHexEdit->setTextInteractionFlags(Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse);

  qtLayout = new QHBoxLayout;
  qtLayout->setAlignment(Qt::AlignRight);
  qtLayout->setMargin(0);
  qtLayout->setSpacing(0);
  qtHexEdit->setLayout(qtLayout);

  qtScrollBar = new QtHexEditScrollBar(*this);
  qtScrollBar->setSingleStep(1);
  qtLayout->addWidget(qtScrollBar);

  qtScrollBar->connect(qtScrollBar, SIGNAL(actionTriggered(int)), SLOT(onScroll()));

  pWidget::construct();
  setBackgroundColor(state().backgroundColor);
  setForegroundColor(state().foregroundColor);
  _setState();
}

auto pHexEdit::destruct() -> void {
  delete qtScrollBar;
  delete qtLayout;
  delete qtHexEdit;
  qtWidget = qtHexEdit = nullptr;
  qtLayout = nullptr;
  qtScrollBar = nullptr;
}

auto pHexEdit::setAddress(unsigned address) -> void {
  _setState();
}

auto pHexEdit::setBackgroundColor(Color color) -> void {
  static auto defaultColor = qtHexEdit->palette().color(QPalette::Base);

  auto palette = qtHexEdit->palette();
  palette.setColor(QPalette::Base, CreateColor(color, defaultColor));
  qtHexEdit->setPalette(palette);
  qtHexEdit->setAutoFillBackground((bool)color);
}

auto pHexEdit::setColumns(unsigned columns) -> void {
  _setState();
}

auto pHexEdit::setForegroundColor(Color color) -> void {
  static auto defaultColor = qtHexEdit->palette().color(QPalette::Text);

  auto palette = qtHexEdit->palette();
  palette.setColor(QPalette::Text, color ? CreateColor(color) : defaultColor);
  qtHexEdit->setPalette(palette);
}

auto pHexEdit::setLength(unsigned length) -> void {
  _setState();
}

auto pHexEdit::setRows(unsigned rows) -> void {
  _setState();
}

auto pHexEdit::update() -> void {
  if(!state().onRead) {
    qtHexEdit->setPlainText("");
    return;
  }

  unsigned cursorPosition = qtHexEdit->textCursor().position();

  string output;
  unsigned address = state().address;
  for(unsigned row = 0; row < state().rows; row++) {
    output.append(hex(address, 8L));
    output.append("  ");

    string hexdata;
    string ansidata = " ";

    for(unsigned column = 0; column < state().columns; column++) {
      if(address < state().length) {
        uint8_t data = self().doRead(address++);
        hexdata.append(hex(data, 2L));
        hexdata.append(" ");
        ansidata.append(data >= 0x20 && data <= 0x7e ? (char)data : '.');
      } else {
        hexdata.append("   ");
        ansidata.append(" ");
      }
    }

    output.append(hexdata);
    output.append(ansidata);
    if(address >= state().length) break;
    if(row != state().rows - 1) output.append("\n");
  }

  qtHexEdit->setPlainText(QString::fromUtf8(output));
  QTextCursor cursor = qtHexEdit->textCursor();
  cursor.setPosition(cursorPosition);
  qtHexEdit->setTextCursor(cursor);
}

auto pHexEdit::_keyPressEvent(QKeyEvent* event) -> void {
  if(!state().onRead) return;

  //allow Ctrl+C (copy)
  if(event->key() == Qt::Key_C && event->modifiers() == Qt::ControlModifier) {
    qtHexEdit->keyPressEventAcknowledge(event);
    return;
  }

  //disallow other text operations (cut, paste, etc)
  if(event->modifiers() & (Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier)) return;

  QTextCursor cursor = qtHexEdit->textCursor();
  signed lineWidth = 10 + (state().columns * 3) + 1 + state().columns + 1;
  signed cursorY = cursor.position() / lineWidth;
  signed cursorX = cursor.position() % lineWidth;

  unsigned nibble = 0;
  switch(event->key()) {
  default: return;

  case Qt::Key_Left:
    if(cursorX > 0) {
      cursor.setPosition(cursor.position() - 1);
      qtHexEdit->setTextCursor(cursor);
    }
    return;

  case Qt::Key_Right:
    if(cursorX < lineWidth - 1) {
      cursor.setPosition(cursor.position() + 1);
      qtHexEdit->setTextCursor(cursor);
    }
    return;

  case Qt::Key_Home:
    cursor.setPosition(cursorY * lineWidth + 10);
    qtHexEdit->setTextCursor(cursor);
    return;

  case Qt::Key_End:
    cursor.setPosition(cursorY * lineWidth + 57);
    qtHexEdit->setTextCursor(cursor);
    return;

  case Qt::Key_Up:
    if(cursorY > 0) {
      cursor.setPosition(cursor.position() - lineWidth);
      qtHexEdit->setTextCursor(cursor);
    } else {
      _scrollTo(qtScrollBar->sliderPosition() - 1);
    }
    return;

  case Qt::Key_Down:
    if(cursorY >= _rows() - 1) {
      //cannot scroll down further
    } else if(cursorY < state().rows - 1) {
      cursor.setPosition(cursor.position() + lineWidth);
      qtHexEdit->setTextCursor(cursor);
    } else {
      _scrollTo(qtScrollBar->sliderPosition() + 1);
    }
    return;

  case Qt::Key_PageUp:
    _scrollTo(qtScrollBar->sliderPosition() - state().rows);
    return;

  case Qt::Key_PageDown:
    _scrollTo(qtScrollBar->sliderPosition() + state().rows);
    return;

  case Qt::Key_0: nibble =  0; break;
  case Qt::Key_1: nibble =  1; break;
  case Qt::Key_2: nibble =  2; break;
  case Qt::Key_3: nibble =  3; break;
  case Qt::Key_4: nibble =  4; break;
  case Qt::Key_5: nibble =  5; break;
  case Qt::Key_6: nibble =  6; break;
  case Qt::Key_7: nibble =  7; break;
  case Qt::Key_8: nibble =  8; break;
  case Qt::Key_9: nibble =  9; break;
  case Qt::Key_A: nibble = 10; break;
  case Qt::Key_B: nibble = 11; break;
  case Qt::Key_C: nibble = 12; break;
  case Qt::Key_D: nibble = 13; break;
  case Qt::Key_E: nibble = 14; break;
  case Qt::Key_F: nibble = 15; break;
  }

  if(cursorX >= 10) {
    //not on an offset
    cursorX -= 10;
    if((cursorX % 3) != 2) {
      //not on a space
      bool cursorNibble = (cursorX % 3) == 1;  //0 = high, 1 = low
      cursorX /= 3;
      if(cursorX < state().columns) {
        //not in ANSI region
        unsigned address = state().address + (cursorY * state().columns + cursorX);

        if(address >= state().length) return;  //do not edit past end of file
        uint8_t data = self().doRead(address);

        //write modified value
        if(cursorNibble == 1) {
          data = (data & 0xf0) | (nibble << 0);
        } else {
          data = (data & 0x0f) | (nibble << 4);
        }
        self().doWrite(address, data);

        //auto-advance cursor to next nibble/byte
        unsigned step = 1;
        if(cursorNibble && cursorX != state().columns - 1) step = 2;
        cursor.setPosition(cursor.position() + step);
        qtHexEdit->setTextCursor(cursor);

        //refresh output to reflect modified data
        update();
      }
    }
  }
}

//number of actual rows
auto pHexEdit::_rows() -> signed {
  return (max(1u, state().length) + state().columns - 1) / state().columns;
}

//number of scrollable row positions
auto pHexEdit::_rowsScrollable() -> signed {
  return max(0u, _rows() - state().rows);
}

auto pHexEdit::_scrollTo(signed position) -> void {
  if(position > _rowsScrollable()) position = _rowsScrollable();
  if(position < 0) position = 0;
  qtScrollBar->setSliderPosition(position);
}

auto pHexEdit::_setState() -> void {
  auto lock = acquire();
  //add one if last row is not equal to column length (eg only part of the row is present)
  bool indivisible = state().columns == 0 || (state().length % state().columns) != 0;
  qtScrollBar->setRange(0, state().length / state().columns + indivisible - state().rows);
  qtScrollBar->setSliderPosition(state().address / state().columns);
  qtScrollBar->setPageStep(state().rows);
  update();
}

auto QtHexEdit::keyPressEvent(QKeyEvent* event) -> void {
  p._keyPressEvent(event);
}

auto QtHexEdit::keyPressEventAcknowledge(QKeyEvent* event) -> void {
  QTextEdit::keyPressEvent(event);
}

auto QtHexEdit::wheelEvent(QWheelEvent* event) -> void {
  auto angleDelta = event->angleDelta();
  if(qAbs(angleDelta.x()) <= qAbs(angleDelta.y())) {
    signed offset = angleDelta.y() < 0 ? +1 : -1;
    p._scrollTo(p.qtScrollBar->sliderPosition() + offset);
    event->accept();
  }
}

auto QtHexEditScrollBar::event(QEvent* event) -> bool {
  if(event->type() == QEvent::Wheel) {
    auto wheelEvent = (QWheelEvent*)event;
    auto angleDelta = wheelEvent->angleDelta();
    if(qAbs(angleDelta.x()) <= qAbs(angleDelta.y())) {
      signed offset = angleDelta.y() < 0 ? +1 : -1;
      p._scrollTo(sliderPosition() + offset);
      return true;
    }
  }
  return QScrollBar::event(event);
}

auto QtHexEditScrollBar::onScroll() -> void {
  if(p.locked()) return;
  unsigned address = sliderPosition();
  p.state().address = address * p.state().columns;
  p.update();
}

}

#endif
