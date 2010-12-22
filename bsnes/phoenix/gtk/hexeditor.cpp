static bool HexEditor_keyPress(GtkWidget *widget, GdkEventKey *event, HexEditor *self) {
  return self->keyPress(event->keyval);
}

static bool HexEditor_scroll(GtkRange *range, GtkScrollType scroll, gdouble value, HexEditor *self) {
  self->scroll((unsigned)value);
  return false;
}

void HexEditor::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height) {
  widget->parent = &parent;

  hexEditor->size = 0;
  hexEditor->offset = 0;
  hexEditor->columns = 16;
  hexEditor->rows = 16;

  object->widget = gtk_hbox_new(false, 0);
  gtk_widget_set_size_request(object->widget, width, height);

  hexEditor->container = gtk_scrolled_window_new(0, 0);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(hexEditor->container), GTK_POLICY_NEVER, GTK_POLICY_NEVER);
  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(hexEditor->container), GTK_SHADOW_ETCHED_IN);

  hexEditor->widget = gtk_text_view_new();
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(hexEditor->widget), GTK_WRAP_NONE);
  gtk_container_add(GTK_CONTAINER(hexEditor->container), hexEditor->widget);
  g_signal_connect(G_OBJECT(hexEditor->widget), "key-press-event", G_CALLBACK(HexEditor_keyPress), (gpointer)this);

  hexEditor->scroll = gtk_vscrollbar_new((GtkAdjustment*)0);
  gtk_range_set_range(GTK_RANGE(hexEditor->scroll), 0, 256);
  gtk_range_set_increments(GTK_RANGE(hexEditor->scroll), 1, 16);
  gtk_widget_set_sensitive(hexEditor->scroll, false);
  g_signal_connect(G_OBJECT(hexEditor->scroll), "change-value", G_CALLBACK(HexEditor_scroll), (gpointer)this);

  gtk_box_pack_start(GTK_BOX(object->widget), hexEditor->container, true, true, 0);
  gtk_box_pack_start(GTK_BOX(object->widget), hexEditor->scroll, false, false, 1);

  object->textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(hexEditor->widget));
  hexEditor->cursor = gtk_text_buffer_get_mark(object->textBuffer, "insert");

  gtk_fixed_put(GTK_FIXED(parent.object->formContainer), object->widget, x, y);
  if(parent.window->defaultFont) setFont(*parent.window->defaultFont);

  gtk_widget_show(hexEditor->scroll);
  gtk_widget_show(hexEditor->widget);
  gtk_widget_show(hexEditor->container);
  gtk_widget_show(object->widget);
}

void HexEditor::setSize(unsigned size) {
  hexEditor->size = size;
  setScroll();
}

void HexEditor::setOffset(unsigned offset) {
  hexEditor->offset = offset;
  setScroll();
  updateScroll();
}

void HexEditor::setColumns(unsigned columns) {
  hexEditor->columns = columns;
  setScroll();
}

void HexEditor::setRows(unsigned rows) {
  hexEditor->rows = rows;
  setScroll();
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

  return true;
}

void HexEditor::scroll(unsigned position) {
  unsigned rows = hexEditor->size / hexEditor->columns;
  if(position >= rows) position = rows - 1;
  setOffset(position * hexEditor->columns);
  update();
}

void HexEditor::setScroll() {
  unsigned rows = hexEditor->size / hexEditor->columns;
  if(rows) rows--;
  if(rows) {
    gtk_range_set_range(GTK_RANGE(hexEditor->scroll), 0, rows);
    gtk_widget_set_sensitive(hexEditor->scroll, true);
  } else {
    gtk_widget_set_sensitive(hexEditor->scroll, false);
  }
}

void HexEditor::updateScroll() {
  unsigned row = hexEditor->offset / hexEditor->columns;
  gtk_range_set_value(GTK_RANGE(hexEditor->scroll), row);
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
