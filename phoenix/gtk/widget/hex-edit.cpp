namespace phoenix {

static bool HexEdit_keyPress(GtkWidget* widget, GdkEventKey* event, HexEdit* self) {
  return self->p.keyPress(event->keyval, event->state);
}

static bool HexEdit_mouseScroll(GtkWidget* widget, GdkEventScroll* event, HexEdit* self) {
  double position = gtk_range_get_value(GTK_RANGE(self->p.scrollBar));

  if(event->direction == GDK_SCROLL_UP) {
    self->p.scroll(position - 1);
  }

  if(event->direction == GDK_SCROLL_DOWN) {
    self->p.scroll(position + 1);
  }

  return true;  //do not propagate event further
}

static bool HexEdit_scroll(GtkRange* range, GtkScrollType scroll, double value, HexEdit* self) {
  self->p.scroll((signed)value);
  return true;  //do not propagate event further
}

bool pHexEdit::focused() {
  return GTK_WIDGET_HAS_FOCUS(subWidget) || GTK_WIDGET_HAS_FOCUS(scrollBar);
}

void pHexEdit::setColumns(unsigned columns) {
  setScroll();
  update();
}

void pHexEdit::setLength(unsigned length) {
  setScroll();
  update();
}

void pHexEdit::setOffset(unsigned offset) {
  setScroll();
  updateScroll();
  update();
}

void pHexEdit::setRows(unsigned rows) {
  setScroll();
  update();
}

void pHexEdit::update() {
  if(!hexEdit.onRead) {
    gtk_text_buffer_set_text(textBuffer, "", -1);
    return;
  }

  unsigned position = cursorPosition();

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

  gtk_text_buffer_set_text(textBuffer, output, -1);
  if(position == 0) position = 10;  //start at first position where hex values can be entered
  setCursorPosition(position);
}

void pHexEdit::constructor() {
  gtkWidget = gtk_hbox_new(false, 0);

  container = gtk_scrolled_window_new(0, 0);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(container), GTK_POLICY_AUTOMATIC, GTK_POLICY_NEVER);
  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(container), GTK_SHADOW_ETCHED_IN);

  subWidget = gtk_text_view_new();
  gtk_text_view_set_editable(GTK_TEXT_VIEW(subWidget), false);
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(subWidget), GTK_WRAP_NONE);
  gtk_container_add(GTK_CONTAINER(container), subWidget);
  g_signal_connect(G_OBJECT(subWidget), "key-press-event", G_CALLBACK(HexEdit_keyPress), (gpointer)&hexEdit);
  g_signal_connect(G_OBJECT(subWidget), "scroll-event", G_CALLBACK(HexEdit_mouseScroll), (gpointer)&hexEdit);

  scrollBar = gtk_vscrollbar_new((GtkAdjustment*)nullptr);
  gtk_range_set_range(GTK_RANGE(scrollBar), 0, 255);
  gtk_range_set_increments(GTK_RANGE(scrollBar), 1, 16);
  gtk_widget_set_sensitive(scrollBar, false);
  g_signal_connect(G_OBJECT(scrollBar), "change-value", G_CALLBACK(HexEdit_scroll), (gpointer)&hexEdit);
  g_signal_connect(G_OBJECT(scrollBar), "scroll-event", G_CALLBACK(HexEdit_mouseScroll), (gpointer)&hexEdit);

  gtk_box_pack_start(GTK_BOX(gtkWidget), container, true, true, 0);
  gtk_box_pack_start(GTK_BOX(gtkWidget), scrollBar, false, false, 1);

  textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(subWidget));
  textCursor = gtk_text_buffer_get_mark(textBuffer, "insert");

  gtk_widget_show(scrollBar);
  gtk_widget_show(subWidget);
  gtk_widget_show(container);

  setColumns(hexEdit.state.columns);
  setRows(hexEdit.state.rows);
  setLength(hexEdit.state.length);
  setOffset(hexEdit.state.offset);
  update();
}

void pHexEdit::destructor() {
  gtk_widget_destroy(scrollBar);
  gtk_widget_destroy(subWidget);
  gtk_widget_destroy(container);
  gtk_widget_destroy(gtkWidget);
}

void pHexEdit::orphan() {
  destructor();
  constructor();
}

unsigned pHexEdit::cursorPosition() {
  GtkTextIter iter;
  gtk_text_buffer_get_iter_at_mark(textBuffer, &iter, textCursor);
  return gtk_text_iter_get_offset(&iter);
}

bool pHexEdit::keyPress(unsigned scancode, unsigned mask) {
  if(!hexEdit.onRead) return false;

  if(mask & (GDK_CONTROL_MASK | GDK_MOD1_MASK | GDK_SUPER_MASK)) return false;  //allow actions such as Ctrl+C (copy)

  signed position = cursorPosition();
  signed lineWidth = 10 + (hexEdit.state.columns * 3) + 1 + hexEdit.state.columns + 1;
  signed cursorY = position / lineWidth;
  signed cursorX = position % lineWidth;

  if(scancode == GDK_Home) {
    setCursorPosition(cursorY * lineWidth + 10);
    return true;
  }

  if(scancode == GDK_End) {
    setCursorPosition(cursorY * lineWidth + 10 + (hexEdit.state.columns * 3 - 1));
    return true;
  }

  if(scancode == GDK_Up) {
    if(cursorY != 0) return false;

    signed newOffset = hexEdit.state.offset - hexEdit.state.columns;
    if(newOffset >= 0) {
      hexEdit.setOffset(newOffset);
      update();
    }
    return true;
  }

  if(scancode == GDK_Down) {
    if(cursorY >= rows() - 1) return true;
    if(cursorY != hexEdit.state.rows - 1) return false;

    signed newOffset = hexEdit.state.offset + hexEdit.state.columns;
    if(newOffset + hexEdit.state.columns * hexEdit.state.rows - (hexEdit.state.columns - 1) <= hexEdit.state.length) {
      hexEdit.setOffset(newOffset);
      update();
    }
    return true;
  }

  if(scancode == GDK_Page_Up) {
    signed newOffset = hexEdit.state.offset - hexEdit.state.columns * hexEdit.state.rows;
    if(newOffset >= 0) {
      hexEdit.setOffset(newOffset);
    } else {
      hexEdit.setOffset(0);
    }
    update();
    return true;
  }

  if(scancode == GDK_Page_Down) {
    signed newOffset = hexEdit.state.offset + hexEdit.state.columns * hexEdit.state.rows;
    for(unsigned n = 0; n < hexEdit.state.rows; n++) {
      if(newOffset + hexEdit.state.columns * hexEdit.state.rows - (hexEdit.state.columns - 1) <= hexEdit.state.length) {
        hexEdit.setOffset(newOffset);
        update();
        break;
      }
      newOffset -= hexEdit.state.columns;
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
      if(cursorX < hexEdit.state.columns) {
        //not in ANSI region
        unsigned offset = hexEdit.state.offset + (cursorY * hexEdit.state.columns + cursorX);

        if(offset >= hexEdit.state.length) return false;  //do not edit past end of data
        uint8_t data = hexEdit.onRead(offset);

        //write modified value
        if(cursorNibble == 1) {
          data = (data & 0xf0) | (scancode << 0);
        } else {
          data = (data & 0x0f) | (scancode << 4);
        }
        if(hexEdit.onWrite) hexEdit.onWrite(offset, data);

        //auto-advance cursor to next nibble/byte
        position++;
        if(cursorNibble && cursorX != hexEdit.state.columns - 1) position++;
        setCursorPosition(position);

        //refresh output to reflect modified data
        update();
      }
    }
  }

  return true;
}

//number of actual rows
signed pHexEdit::rows() {
  return (max(1u, hexEdit.state.length) + hexEdit.state.columns - 1) / hexEdit.state.columns;
}

//number of scrollable row positions
signed pHexEdit::rowsScrollable() {
  return max(0u, rows() - hexEdit.state.rows);
}

void pHexEdit::scroll(signed position) {
  if(position > rowsScrollable()) position = rowsScrollable();
  if(position < 0) position = 0;
  hexEdit.setOffset(position * hexEdit.state.columns);
}

void pHexEdit::setCursorPosition(unsigned position) {
  GtkTextIter iter;
  gtk_text_buffer_get_iter_at_mark(textBuffer, &iter, textCursor);

  //GTK+ will throw many errors to the terminal if you set iterator past end of buffer
  GtkTextIter endIter;
  gtk_text_buffer_get_end_iter(textBuffer, &iter);
  unsigned endPosition = gtk_text_iter_get_offset(&iter);

  gtk_text_iter_set_offset(&iter, min(position, endPosition));
  gtk_text_buffer_place_cursor(textBuffer, &iter);
}

void pHexEdit::setScroll() {
  if(rowsScrollable() > 0) {
    gtk_range_set_range(GTK_RANGE(scrollBar), 0, rowsScrollable());
    gtk_widget_set_sensitive(scrollBar, true);
  } else {
    gtk_widget_set_sensitive(scrollBar, false);
  }
}

void pHexEdit::updateScroll() {
  unsigned row = hexEdit.state.offset / hexEdit.state.columns;
  gtk_range_set_value(GTK_RANGE(scrollBar), row);
}

}
