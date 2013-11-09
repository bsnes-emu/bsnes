namespace phoenix {

void pHexEdit::setColumns(unsigned columns) {
  update();
}

void pHexEdit::setLength(unsigned length) {
  //add one if last row is not equal to column length (eg only part of the row is present)
  bool indivisible = hexEdit.state.columns == 0 || (hexEdit.state.length % hexEdit.state.columns) != 0;
  qtScroll->setRange(0, hexEdit.state.length / hexEdit.state.columns + indivisible - hexEdit.state.rows);
  update();
}

void pHexEdit::setOffset(unsigned offset) {
  locked = true;
  qtScroll->setSliderPosition(hexEdit.state.offset / hexEdit.state.columns);
  locked = false;
  update();
}

void pHexEdit::setRows(unsigned rows) {
  qtScroll->setPageStep(hexEdit.state.rows);
  update();
}

void pHexEdit::update() {
  if(!hexEdit.onRead) {
    qtHexEdit->setPlainText("");
    return;
  }

  unsigned cursorPosition = qtHexEdit->textCursor().position();

  string output;
  unsigned offset = hexEdit.state.offset;
  for(unsigned row = 0; row < hexEdit.state.rows; row++) {
    output.append(hex<8>(offset));
    output.append("  ");

    string hexdata;
    string ansidata = " ";

    for(unsigned column = 0; column < hexEdit.state.columns; column++) {
      if(offset < hexEdit.state.length) {
        uint8_t data = hexEdit.onRead(offset++);
        hexdata.append(hex<2>(data));
        hexdata.append(" ");
        ansidata.append(data >= 0x20 && data <= 0x7e ? (char)data : '.');
      } else {
        hexdata.append("   ");
        ansidata.append(" ");
      }
    }

    output.append(hexdata);
    output.append(ansidata);
    if(offset >= hexEdit.state.length) break;
    if(row != hexEdit.state.rows - 1) output.append("\n");
  }

  qtHexEdit->setPlainText(QString::fromUtf8(output));
  QTextCursor cursor = qtHexEdit->textCursor();
  cursor.setPosition(cursorPosition);
  qtHexEdit->setTextCursor(cursor);
}

void pHexEdit::constructor() {
  qtWidget = qtHexEdit = new QtHexEdit(*this);

  qtHexEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  qtHexEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  qtHexEdit->setTextInteractionFlags(Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse);

  qtLayout = new QHBoxLayout;
  qtLayout->setAlignment(Qt::AlignRight);
  qtLayout->setMargin(0);
  qtLayout->setSpacing(0);
  qtHexEdit->setLayout(qtLayout);

  qtScroll = new QtHexEditScrollBar(*this);
  qtScroll->setSingleStep(1);
  qtLayout->addWidget(qtScroll);

  connect(qtScroll, SIGNAL(actionTriggered(int)), SLOT(onScroll()));

  pWidget::synchronizeState();
  setColumns(hexEdit.state.columns);
  setRows(hexEdit.state.rows);
  setLength(hexEdit.state.length);
  setOffset(hexEdit.state.offset);
  update();
}

void pHexEdit::destructor() {
  delete qtScroll;
  delete qtLayout;
  delete qtHexEdit;
  qtWidget = qtHexEdit = nullptr;
  qtLayout = nullptr;
  qtScroll = nullptr;
}

void pHexEdit::orphan() {
  destructor();
  constructor();
}

void pHexEdit::keyPressEvent(QKeyEvent* event) {
  if(!hexEdit.onRead) return;

  //allow Ctrl+C (copy)
  if(event->key() == Qt::Key_C && event->modifiers() == Qt::ControlModifier) {
    qtHexEdit->keyPressEventAcknowledge(event);
    return;
  }

  //disallow other text operations (cut, paste, etc)
  if(event->modifiers() & (Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier)) return;

  QTextCursor cursor = qtHexEdit->textCursor();
  signed lineWidth = 10 + (hexEdit.state.columns * 3) + 1 + hexEdit.state.columns + 1;
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
      scrollTo(qtScroll->sliderPosition() - 1);
    }
    return;

  case Qt::Key_Down:
    if(cursorY >= rows() - 1) {
      //cannot scroll down further
    } else if(cursorY < hexEdit.state.rows - 1) {
      cursor.setPosition(cursor.position() + lineWidth);
      qtHexEdit->setTextCursor(cursor);
    } else {
      scrollTo(qtScroll->sliderPosition() + 1);
    }
    return;

  case Qt::Key_PageUp:
    scrollTo(qtScroll->sliderPosition() - hexEdit.state.rows);
    return;

  case Qt::Key_PageDown:
    scrollTo(qtScroll->sliderPosition() + hexEdit.state.rows);
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
      if(cursorX < hexEdit.state.columns) {
        //not in ANSI region
        unsigned offset = hexEdit.state.offset + (cursorY * hexEdit.state.columns + cursorX);

        if(offset >= hexEdit.state.length) return;  //do not edit past end of file
        uint8_t data = hexEdit.onRead(offset);

        //write modified value
        if(cursorNibble == 1) {
          data = (data & 0xf0) | (nibble << 0);
        } else {
          data = (data & 0x0f) | (nibble << 4);
        }
        if(hexEdit.onWrite) hexEdit.onWrite(offset, data);

        //auto-advance cursor to next nibble/byte
        unsigned step = 1;
        if(cursorNibble && cursorX != hexEdit.state.columns - 1) step = 2;
        cursor.setPosition(cursor.position() + step);
        qtHexEdit->setTextCursor(cursor);

        //refresh output to reflect modified data
        update();
      }
    }
  }
}

//number of actual rows
signed pHexEdit::rows() {
  return (max(1u, hexEdit.state.length) + hexEdit.state.columns - 1) / hexEdit.state.columns;
}

//number of scrollable row positions
signed pHexEdit::rowsScrollable() {
  return max(0u, rows() - hexEdit.state.rows);
}

void pHexEdit::scrollTo(signed position) {
  if(position > rowsScrollable()) position = rowsScrollable();
  if(position < 0) position = 0;
  qtScroll->setSliderPosition(position);
}

void pHexEdit::onScroll() {
  if(locked) return;
  unsigned offset = qtScroll->sliderPosition();
  hexEdit.state.offset = offset * hexEdit.state.columns;
  update();
}

void pHexEdit::QtHexEdit::keyPressEvent(QKeyEvent* event) {
  self.keyPressEvent(event);
}

void pHexEdit::QtHexEdit::keyPressEventAcknowledge(QKeyEvent* event) {
  QTextEdit::keyPressEvent(event);
}

void pHexEdit::QtHexEdit::wheelEvent(QWheelEvent* event) {
  if(event->orientation() == Qt::Vertical) {
    signed offset = event->delta() < 0 ? +1 : -1;
    self.scrollTo(self.qtScroll->sliderPosition() + offset);
    event->accept();
  }
}

bool pHexEdit::QtHexEditScrollBar::event(QEvent* event) {
  if(event->type() == QEvent::Wheel) {
    QWheelEvent* wheelEvent = (QWheelEvent*)event;
    if(wheelEvent->orientation() == Qt::Vertical) {
      signed offset = wheelEvent->delta() < 0 ? +1 : -1;
      self.scrollTo(sliderPosition() + offset);
      return true;
    }
  }
  return QScrollBar::event(event);
}

}
