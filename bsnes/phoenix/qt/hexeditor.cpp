void HexEditor::setSize(unsigned size) {
  hexEditor->size = size;
  bool indivisible = (hexEditor->size % hexEditor->columns) != 0;  //add one for incomplete row
  hexEditor->scrollBar->setRange(0, hexEditor->size / hexEditor->columns + indivisible - hexEditor->rows);
}

void HexEditor::setOffset(unsigned offset) {
  object->locked = true;
  hexEditor->offset = offset;
  hexEditor->scrollBar->setSliderPosition(hexEditor->offset / hexEditor->columns);
  object->locked = false;
}

void HexEditor::setColumns(unsigned columns) {
  hexEditor->columns = columns;
}

void HexEditor::setRows(unsigned rows) {
  hexEditor->rows = rows;
  hexEditor->scrollBar->setPageStep(hexEditor->rows);
}

void HexEditor::update() {
  if(!onRead) {
    hexEditor->setPlainText("");
    return;
  }

  unsigned cursorPosition = hexEditor->textCursor().position();

  string output;
  unsigned offset = hexEditor->offset;
  for(unsigned row = 0; row < hexEditor->rows; row++) {
    output.append(hex<8>(offset));
    output.append("  ");

    string hexdata;
    string ansidata = " ";
    for(unsigned column = 0; column < hexEditor->columns; column++) {
      if(offset < hexEditor->size) {
        uint8_t data = onRead(offset++);
        hexdata.append(hex<2>(data));
        hexdata.append(" ");
        char buffer[2] = { data >= 0x20 && data <= 0x7e ? (char)data : '.', 0 };
        ansidata.append(buffer);
      } else {
        hexdata.append("   ");
        ansidata.append(" ");
      }
    }

    output.append(hexdata);
    output.append(ansidata);
    if(offset >= hexEditor->size) break;
    if(row != hexEditor->rows - 1) output.append("\n");
  }

  hexEditor->setPlainText((const char*)output);
  QTextCursor cursor = hexEditor->textCursor();
  cursor.setPosition(cursorPosition);
  hexEditor->setTextCursor(cursor);
}

void HexEditor::Data::keyPressEvent(QKeyEvent *event) {
  if(!self.onRead || !self.onWrite) return;

  QTextCursor cursor = textCursor();
  unsigned lineWidth = 10 + (columns * 3) + 1 + (columns) + 1;
  unsigned cursorY = cursor.position() / lineWidth;
  unsigned cursorX = cursor.position() % lineWidth;

  unsigned nibble;
  switch(event->key()) {
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
    default: {
      //allow navigation keys to move cursor, but block text input
      setTextInteractionFlags(Qt::TextInteractionFlags(
        Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse
      ));
      QTextEdit::keyPressEvent(event);
      setTextInteractionFlags(Qt::TextEditorInteraction);
      return;
    }
  }

  if(cursorX >= 10) {
    //not on an offset
    cursorX -= 10;
    if((cursorX % 3) != 2) {
      //not on a space
      bool cursorNibble = (cursorX % 3) == 1;  //0 = high, 1 = low
      cursorX /= 3;
      if(cursorX < columns) {
        //not in ANSI region
        unsigned currentOffset = offset + (cursorY * columns + cursorX);

        if(currentOffset >= size) return;  //do not edit past end of file
        uint8_t data = self.onRead(currentOffset);

        //write modified value
        if(cursorNibble == 1) {
          data = (data & 0xf0) | (nibble << 0);
        } else {
          data = (data & 0x0f) | (nibble << 4);
        }
        self.onWrite(currentOffset, data);

        //auto-advance cursor to next nibble/byte
        unsigned step = 1;
        if(cursorNibble && cursorX != columns - 1) step = 2;
        cursor.setPosition(cursor.position() + step);
        setTextCursor(cursor);

        //refresh output to reflect modified data
        self.update();
      }
    }
  }
}

void HexEditor::Data::scrollEvent() {
  if(self.object->locked) return;
  unsigned currentOffset = scrollBar->sliderPosition();
  offset = currentOffset * columns;
  self.update();
}

HexEditor::HexEditor() {
  hexEditor = new HexEditor::Data(*this);
  widget->widget = hexEditor;

  hexEditor->size = 0;
  hexEditor->offset = 0;
  hexEditor->columns = 16;
  hexEditor->rows = 16;

  hexEditor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  hexEditor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  hexEditor->layout = new QHBoxLayout;
  hexEditor->layout->setAlignment(Qt::AlignRight);
  hexEditor->layout->setMargin(0);
  hexEditor->layout->setSpacing(0);
  hexEditor->setLayout(hexEditor->layout);

  hexEditor->scrollBar = new QScrollBar(Qt::Vertical);
  hexEditor->scrollBar->setSingleStep(1);
  hexEditor->layout->addWidget(hexEditor->scrollBar);

  hexEditor->scrollBar->connect(
    hexEditor->scrollBar, SIGNAL(actionTriggered(int)), hexEditor, SLOT(scrollEvent())
  );
}
