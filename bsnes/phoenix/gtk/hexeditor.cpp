static bool HexEditor_keyPress(GtkWidget *widget, GdkEventKey *event, HexEditor *self) {
  return self->keyPress(event->keyval);
}

void HexEditor::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height) {
  object->widget = gtk_scrolled_window_new(0, 0);
  widget->parent = &parent;
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(object->widget), GTK_POLICY_NEVER, GTK_POLICY_NEVER);
  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(object->widget), GTK_SHADOW_ETCHED_IN);
  gtk_widget_set_size_request(object->widget, width, height);
  object->subWidget = gtk_text_view_new();
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(object->subWidget), GTK_WRAP_NONE);
  gtk_container_add(GTK_CONTAINER(object->widget), object->subWidget);
  object->textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(object->subWidget));
  hexEditor->cursor = gtk_text_buffer_get_mark(object->textBuffer, "insert");

  gtk_text_view_set_editable(GTK_TEXT_VIEW(object->subWidget), false);
  g_signal_connect(G_OBJECT(object->subWidget), "key-press-event", G_CALLBACK(HexEditor_keyPress), (gpointer)this);

  if(parent.window->defaultFont) setFont(*parent.window->defaultFont);
  gtk_fixed_put(GTK_FIXED(parent.object->formContainer), object->widget, x, y);
  gtk_widget_show(object->subWidget);
  gtk_widget_show(object->widget);
}

void HexEditor::setSize(unsigned size) {
  hexEditor->size = size;
}

void HexEditor::setOffset(unsigned offset) {
  hexEditor->offset = offset;
}

void HexEditor::setColumns(unsigned columns) {
  hexEditor->columns = columns;
}

void HexEditor::setRows(unsigned rows) {
  hexEditor->rows = rows;
}

void HexEditor::update() {
  if(!onRead) {
    gtk_text_buffer_set_text(object->textBuffer, "", -1);
    return;
  }

  unsigned position = cursorPosition();

  string output;
  unsigned offset = hexEditor->offset;
  for(unsigned row = 0; row < hexEditor->rows; row++) {
    output.append(strhex<8>(offset));
    output.append("  ");

    string hexdata;
    string ansidata = " ";
    for(unsigned column = 0; column < hexEditor->columns; column++) {
      if(offset < hexEditor->size) {
        uint8_t data = onRead(offset++);
        hexdata.append(strhex<2>(data));
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

  gtk_text_buffer_set_text(object->textBuffer, output, -1);
  if(position == 0) position = 10;  //start at first position where hex values can be entered
  setCursorPosition(position);
}

HexEditor::HexEditor() {
  hexEditor = new HexEditor::Data;
}

//internal

bool HexEditor::keyPress(unsigned scancode) {
  if(!onRead && !onWrite) return false;

  unsigned position = cursorPosition();
  unsigned lineWidth = 10 + (hexEditor->columns * 3) + 1 + (hexEditor->columns) + 1;
  unsigned cursorY = position / lineWidth;
  unsigned cursorX = position % lineWidth;

  if(scancode == GDK_Home) {
    setCursorPosition(cursorY * lineWidth + 10);
    return true;
  }

  if(scancode == GDK_End) {
    setCursorPosition(cursorY * lineWidth + 10 + (hexEditor->columns * 3 - 1));
    return true;
  }

  if(scancode == GDK_Up) {
    if(cursorY != 0) return false;

    signed newOffset = hexEditor->offset - hexEditor->columns;
    if(newOffset >= 0) {
      setOffset(newOffset);
      update();
    }
    return true;
  }

  if(scancode == GDK_Down) {
    if(cursorY != hexEditor->rows - 1) return false;

    signed newOffset = hexEditor->offset + hexEditor->columns;
    if(newOffset + hexEditor->columns * hexEditor->rows - (hexEditor->columns - 1) <= hexEditor->size) {
      setOffset(newOffset);
      update();
    }
    return true;
  }

  if(scancode == GDK_Page_Up) {
    signed newOffset = hexEditor->offset - hexEditor->columns * hexEditor->rows;
    if(newOffset >= 0) {
      setOffset(newOffset);
      update();
    } else {
      setOffset(0);
      update();
    }
    return true;
  }

  if(scancode == GDK_Page_Down) {
    signed newOffset = hexEditor->offset + hexEditor->columns * hexEditor->rows;
    for(unsigned n = 0; n < hexEditor->rows; n++) {
      if(newOffset + hexEditor->columns * hexEditor->rows - (hexEditor->columns - 1) <= hexEditor->size) {
        setOffset(newOffset);
        update();
        break;
      }
      newOffset -= hexEditor->columns;
    }
    return true;
  }

  //convert scancode to hex nibble
       if(scancode >= '0' && scancode <= '9') scancode = scancode - '0';
  else if(scancode >= 'A' && scancode <= 'F') scancode = scancode - 'A' + 10;
  else if(scancode >= 'a' && scancode <= 'f') scancode = scancode - 'a' + 10;
  else return false;  //not a valid hex value

  if(cursorX >= 10) {
    //not on an offset
    cursorX -= 10;
    if((cursorX % 3) != 2) {
      //not on a space
      bool cursorNibble = (cursorX % 3) == 1;  //0 = high, 1 = low
      cursorX /= 3;
      if(cursorX < hexEditor->columns) {
        //not in ANSI region
        unsigned offset = hexEditor->offset + (cursorY * hexEditor->columns + cursorX);

        if(offset >= hexEditor->size) return false;  //do not edit past end of file
        uint8_t data = onRead(offset);

        //write modified value
        if(cursorNibble == 1) {
          data = (data & 0xf0) | (scancode << 0);
        } else {
          data = (data & 0x0f) | (scancode << 4);
        }
        onWrite(offset, data);

        //auto-advance cursor to next nibble/byte
        position++;
        if(cursorNibble && cursorX != hexEditor->columns - 1) position++;
        setCursorPosition(position);

        //refresh output to reflect modified data
        update();
      }
    }
  }

  return false;
}

unsigned HexEditor::cursorPosition() {
  GtkTextIter iter;
  gtk_text_buffer_get_iter_at_mark(object->textBuffer, &iter, hexEditor->cursor);
  return gtk_text_iter_get_offset(&iter);
}

void HexEditor::setCursorPosition(unsigned position) {
  GtkTextIter iter;
  gtk_text_buffer_get_iter_at_mark(object->textBuffer, &iter, hexEditor->cursor);

  //GTK+ will throw a hundred errors on the terminal
  //if you set an iterator past the end of the text buffer
  GtkTextIter endIter;
  gtk_text_buffer_get_end_iter(object->textBuffer, &iter);
  unsigned endPosition = gtk_text_iter_get_offset(&iter);

  gtk_text_iter_set_offset(&iter, min(position, endPosition));
  gtk_text_buffer_place_cursor(object->textBuffer, &iter);
}
