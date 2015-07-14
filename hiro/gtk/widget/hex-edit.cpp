#if defined(Hiro_HexEdit)

namespace hiro {

static auto HexEdit_keyPress(GtkWidget* widget, GdkEventKey* event, pHexEdit* p) -> signed {
  return p->keyPress(event->keyval, event->state);
}

static auto HexEdit_mouseScroll(GtkWidget* widget, GdkEventScroll* event, pHexEdit* p) -> signed {
  double position = gtk_range_get_value(GTK_RANGE(p->scrollBar));

  if(event->direction == GDK_SCROLL_UP) {
    p->scroll(position - 1);
  }

  if(event->direction == GDK_SCROLL_DOWN) {
    p->scroll(position + 1);
  }

  return true;  //do not propagate event further
}

static auto HexEdit_scroll(GtkRange* range, GtkScrollType scroll, double value, pHexEdit* p) -> signed {
  p->scroll((signed)value);
  return true;  //do not propagate event further
}

auto pHexEdit::construct() -> void {
  gtkWidget = gtk_hbox_new(false, 0);

  container = gtk_scrolled_window_new(0, 0);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(container), GTK_POLICY_AUTOMATIC, GTK_POLICY_NEVER);
  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(container), GTK_SHADOW_ETCHED_IN);

  subWidget = gtk_text_view_new();
  gtk_text_view_set_editable(GTK_TEXT_VIEW(subWidget), false);
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(subWidget), GTK_WRAP_NONE);
  gtk_container_add(GTK_CONTAINER(container), subWidget);

  scrollBar = gtk_vscrollbar_new((GtkAdjustment*)nullptr);
  gtk_range_set_range(GTK_RANGE(scrollBar), 0, 255);
  gtk_range_set_increments(GTK_RANGE(scrollBar), 1, 16);
  gtk_widget_set_sensitive(scrollBar, false);

  gtk_box_pack_start(GTK_BOX(gtkWidget), container, true, true, 0);
  gtk_box_pack_start(GTK_BOX(gtkWidget), scrollBar, false, false, 1);

  textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(subWidget));
  textCursor = gtk_text_buffer_get_mark(textBuffer, "insert");

  gtk_widget_show(scrollBar);
  gtk_widget_show(subWidget);
  gtk_widget_show(container);

  setBackgroundColor(state().backgroundColor);
  setColumns(state().columns);
  setForegroundColor(state().foregroundColor);
  setRows(state().rows);
  setLength(state().length);
  setOffset(state().offset);
  update();

  g_signal_connect(G_OBJECT(subWidget), "key-press-event", G_CALLBACK(HexEdit_keyPress), (gpointer)this);
  g_signal_connect(G_OBJECT(subWidget), "scroll-event", G_CALLBACK(HexEdit_mouseScroll), (gpointer)this);

  g_signal_connect(G_OBJECT(scrollBar), "change-value", G_CALLBACK(HexEdit_scroll), (gpointer)this);
  g_signal_connect(G_OBJECT(scrollBar), "scroll-event", G_CALLBACK(HexEdit_mouseScroll), (gpointer)this);

  pWidget::construct();
}

auto pHexEdit::destruct() -> void {
  gtk_widget_destroy(scrollBar);
  gtk_widget_destroy(subWidget);
  gtk_widget_destroy(container);
  gtk_widget_destroy(gtkWidget);
}

auto pHexEdit::focused() const -> bool {
  return GTK_WIDGET_HAS_FOCUS(subWidget) || GTK_WIDGET_HAS_FOCUS(scrollBar);
}

auto pHexEdit::setBackgroundColor(Color color) -> void {
  GdkColor gdkColor = CreateColor(color);
  gtk_widget_modify_base(subWidget, GTK_STATE_NORMAL, color ? &gdkColor : nullptr);
}

auto pHexEdit::setColumns(unsigned columns) -> void {
  setScroll();
  update();
}

auto pHexEdit::setForegroundColor(Color color) -> void {
  GdkColor gdkColor = CreateColor(color);
  gtk_widget_modify_text(subWidget, GTK_STATE_NORMAL, color ? &gdkColor : nullptr);
}

auto pHexEdit::setLength(unsigned length) -> void {
  setScroll();
  update();
}

auto pHexEdit::setOffset(unsigned offset) -> void {
  setScroll();
  updateScroll();
  update();
}

auto pHexEdit::setRows(unsigned rows) -> void {
  setScroll();
  update();
}

auto pHexEdit::update() -> void {
  if(!state().onRead) {
    gtk_text_buffer_set_text(textBuffer, "", -1);
    return;
  }

  unsigned position = cursorPosition();

  string output;
  unsigned offset = state().offset;
  for(auto row : range(state().rows)) {
    output.append(hex(offset, 8L));
    output.append("  ");

    string hexdata;
    string ansidata = " ";
    for(auto column : range(state().columns)) {
      if(offset < state().length) {
        uint8_t data = self().doRead(offset++);
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
    if(offset >= state().length) break;
    if(row != state().rows - 1) output.append("\n");
  }

  gtk_text_buffer_set_text(textBuffer, output, -1);
  if(position == 0) position = 10;  //start at first position where hex values can be entered
  setCursorPosition(position);
}

auto pHexEdit::cursorPosition() -> unsigned {
  GtkTextIter iter;
  gtk_text_buffer_get_iter_at_mark(textBuffer, &iter, textCursor);
  return gtk_text_iter_get_offset(&iter);
}

auto pHexEdit::keyPress(unsigned scancode, unsigned mask) -> bool {
  if(!state().onRead) return false;

  if(mask & (GDK_CONTROL_MASK | GDK_MOD1_MASK | GDK_SUPER_MASK)) return false;  //allow actions such as Ctrl+C (copy)

  signed position = cursorPosition();
  signed lineWidth = 10 + (state().columns * 3) + 1 + state().columns + 1;
  signed cursorY = position / lineWidth;
  signed cursorX = position % lineWidth;

  if(scancode == GDK_Home) {
    setCursorPosition(cursorY * lineWidth + 10);
    return true;
  }

  if(scancode == GDK_End) {
    setCursorPosition(cursorY * lineWidth + 10 + (state().columns * 3 - 1));
    return true;
  }

  if(scancode == GDK_Up) {
    if(cursorY != 0) return false;

    signed newOffset = state().offset - state().columns;
    if(newOffset >= 0) {
      self().setOffset(newOffset);
      update();
    }
    return true;
  }

  if(scancode == GDK_Down) {
    if(cursorY >= rows() - 1) return true;
    if(cursorY != state().rows - 1) return false;

    signed newOffset = state().offset + state().columns;
    if(newOffset + state().columns * state().rows - (state().columns - 1) <= state().length) {
      self().setOffset(newOffset);
      update();
    }
    return true;
  }

  if(scancode == GDK_Page_Up) {
    signed newOffset = state().offset - state().columns * state().rows;
    if(newOffset >= 0) {
      self().setOffset(newOffset);
    } else {
      self().setOffset(0);
    }
    update();
    return true;
  }

  if(scancode == GDK_Page_Down) {
    signed newOffset = state().offset + state().columns * state().rows;
    for(auto n : range(state().rows)) {
      if(newOffset + state().columns * state().rows - (state().columns - 1) <= state().length) {
        self().setOffset(newOffset);
        update();
        break;
      }
      newOffset -= state().columns;
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
      if(cursorX < state().columns) {
        //not in ANSI region
        unsigned offset = state().offset + (cursorY * state().columns + cursorX);

        if(offset >= state().length) return false;  //do not edit past end of data
        uint8_t data = self().doRead(offset);

        //write modified value
        if(cursorNibble == 1) {
          data = (data & 0xf0) | (scancode << 0);
        } else {
          data = (data & 0x0f) | (scancode << 4);
        }
        self().doWrite(offset, data);

        //auto-advance cursor to next nibble/byte
        position++;
        if(cursorNibble && cursorX != state().columns - 1) position++;
        setCursorPosition(position);

        //refresh output to reflect modified data
        update();
      }
    }
  }

  return true;
}

//number of actual rows
auto pHexEdit::rows() -> signed {
  return (max(1u, state().length) + state().columns - 1) / state().columns;
}

//number of scrollable row positions
auto pHexEdit::rowsScrollable() -> signed {
  return max(0u, rows() - state().rows);
}

auto pHexEdit::scroll(signed position) -> void {
  if(position > rowsScrollable()) position = rowsScrollable();
  if(position < 0) position = 0;
  self().setOffset(position * state().columns);
}

auto pHexEdit::setCursorPosition(unsigned position) -> void {
  GtkTextIter iter;
  gtk_text_buffer_get_iter_at_mark(textBuffer, &iter, textCursor);

  //GTK+ will throw many errors to the terminal if you set iterator past end of buffer
  GtkTextIter endIter;
  gtk_text_buffer_get_end_iter(textBuffer, &iter);
  unsigned endPosition = gtk_text_iter_get_offset(&iter);

  gtk_text_iter_set_offset(&iter, min(position, endPosition));
  gtk_text_buffer_place_cursor(textBuffer, &iter);
}

auto pHexEdit::setScroll() -> void {
  if(rowsScrollable() > 0) {
    gtk_range_set_range(GTK_RANGE(scrollBar), 0, rowsScrollable());
    gtk_widget_set_sensitive(scrollBar, true);
  } else {
    gtk_widget_set_sensitive(scrollBar, false);
  }
}

auto pHexEdit::updateScroll() -> void {
  unsigned row = state().offset / state().columns;
  gtk_range_set_value(GTK_RANGE(scrollBar), row);
}

}

#endif
