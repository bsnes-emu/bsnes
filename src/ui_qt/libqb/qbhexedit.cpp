void QbHexEdit::keyPressEvent(QKeyEvent *event) {
  event->accept();

  QTextCursor cursor = textCursor();
  unsigned x = cursor.position() % (editorLineWidth + 1);  //+1 includes right-most cursor position
  unsigned y = cursor.position() / (editorLineWidth + 1);  //eg '0123' has five possible cursor locations

  int hexCode = -1;
  switch(event->key()) {
    case Qt::Key_0: hexCode =  0; break;
    case Qt::Key_1: hexCode =  1; break;
    case Qt::Key_2: hexCode =  2; break;
    case Qt::Key_3: hexCode =  3; break;
    case Qt::Key_4: hexCode =  4; break;
    case Qt::Key_5: hexCode =  5; break;
    case Qt::Key_6: hexCode =  6; break;
    case Qt::Key_7: hexCode =  7; break;
    case Qt::Key_8: hexCode =  8; break;
    case Qt::Key_9: hexCode =  9; break;
    case Qt::Key_A: hexCode = 10; break;
    case Qt::Key_B: hexCode = 11; break;
    case Qt::Key_C: hexCode = 12; break;
    case Qt::Key_D: hexCode = 13; break;
    case Qt::Key_E: hexCode = 14; break;
    case Qt::Key_F: hexCode = 15; break;
  }

  if(hexCode != -1) {
    bool cursorOffsetValid = (x >= 11 && ((x - 11) % 3) != 2);  //cannot input hex on top of offset or blank spaces
    if(cursorOffsetValid) {
      bool nibble = (x - 11) % 3;  //0 = top nibble, 1 = bottom nibble
      unsigned cursorOffset = y * editorColumns + ((x - 11) / 3);
      unsigned effectiveOffset = documentOffset + cursorOffset;
      if(effectiveOffset >= documentSize) effectiveOffset %= documentSize;

      uint8 data = reader(effectiveOffset);
      data &= (nibble == 0 ? 0x0f : 0xf0);
      data |= (nibble == 0 ? (hexCode << 4) : (hexCode << 0));
      writer(effectiveOffset, data);
      update();

      x += (nibble == 0 ? 1 : 2);
    }
  }

  if(event->key() == Qt::Key_Up) {
    if(y > 0) {
      y--;  //move cursor up one row
    } else {
      //scroll document up one row
      if(documentOffset >= editorColumns) {
        documentOffset -= editorColumns;
      } else {
        documentOffset = 0;
      }
      update();
    }
  }

  if(event->key() == Qt::Key_Down) {
    if(y < editorRows - 1) {
      y++;  //move cursor down one row
    } else {
      //scroll document down one row
      if(documentOffset + editorRows * (editorColumns + 1) < documentSize) {
        documentOffset += editorColumns;
      } else {
        documentOffset = documentSize - (editorColumns * editorColumns);
      }
      update();
    }
  }

  if(event->key() == Qt::Key_Left) {
    if(x < 11) {
      x = 11;
    } else if(x == 11) {
      //wrap cursor from left-most to right-most of previous row
      x = editorLineWidth - 1;
      if(y > 0) y--;
    } else {
      x -= (((x - 11) % 3) == 0) ? 2 : 1;
    }
  }

  if(event->key() == Qt::Key_Right) {
    if(x < 11) {
      x = 11;
    } else if(x >= editorLineWidth - 1) {
      //wrap cursor from right-most to left-most of next line
      x = 11;
      if(y < editorRows - 1) y++;
    } else {
      x += (((x - 11) % 3) == 1) ? 2 : 1;
    }
  }

  //normalize cursor to point to hex values
  if(x < 11) x = 11;

  cursor.setPosition(y * (editorLineWidth + 1) + x);
  setTextCursor(cursor);
}

void QbHexEdit::setDocumentSize(unsigned size) {
  documentSize = size;
}

void QbHexEdit::setDocumentOffset(unsigned offset) {
  documentOffset = offset;
  if(documentOffset >= documentSize) documentOffset %= documentSize;
}

void QbHexEdit::setColumns(unsigned columns) {
  editorColumns = columns;
  editorLineWidth = 11 + 3 * columns - 1;  //"0123:4567  00 01 02 03 ... 0f"
}

void QbHexEdit::setRows(unsigned rows) {
  editorRows = rows;
}

void QbHexEdit::update() {
  string output;
  char temp[256];
  unsigned offset = documentOffset;

  for(unsigned y = 0; y < editorRows; y++) {
    sprintf(temp, "%.4x:%.4x", (uint16)(offset >> 16), (uint16)(offset >> 0));
    output << "<font color='#606060'>" << temp << "</font>&nbsp;&nbsp;";

    for(unsigned x = 0; x < editorColumns; x++) {
      sprintf(temp, "%.2x", reader(offset++));
      output << "<font color='" << ((x & 1) ? "#000080" : "#0000ff") << "'>" << temp << "</font>";
      if(offset >= documentSize) goto endOfDocument;
      if(x != editorColumns - 1) output << "&nbsp;";
    }

    if(y != editorRows - 1) output << "<br>";
  } endOfDocument:

  setHtml((const char*)output);

  unsigned scrollMin = 0;
  unsigned scrollMax = documentSize / editorColumns;
  unsigned scrollPos = documentOffset / editorColumns;
  scrollBar->setRange(scrollMin, scrollMax);
  scrollBar->setSliderPosition(0);  //(scrollPos);
}

QbHexEdit::QbHexEdit() {
  scrollBar = new QbHexEditScrollBar;
  scrollBar->setEnabled(false);
  setVerticalScrollBar(scrollBar);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}
