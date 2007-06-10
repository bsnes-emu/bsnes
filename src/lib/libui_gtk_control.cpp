namespace libui {

/*****
 * Control
 *****/

void Control::move(uint x, uint y) {
  gtk_fixed_move(GTK_FIXED(owner->info.container), widget, x, y);
}

void Control::resize(uint width, uint height) {
  gtk_widget_set_size_request(widget, width, height);
}

void Control::focus() {
  gtk_widget_grab_focus(widget);
}

void Control::show() {
  gtk_widget_show(widget);
}

void Control::hide() {
  gtk_widget_hide(widget);
}

void Control::show(bool state) {
  (state == true) ? show() : hide();
}

bool Control::visible() {
  return GTK_WIDGET_VISIBLE(widget);
}

void Control::enable() {
  gtk_widget_set_sensitive(widget, true);
}

void Control::disable() {
  gtk_widget_set_sensitive(widget, false);
}

void Control::enable(bool state) {
  (state == true) ? enable() : disable();
}

bool Control::enabled() {
  return GTK_WIDGET_SENSITIVE(widget);
}

/*****
 * MenuBar
 *****/

void MenuBar::create(Window &r_owner) {
  type   = ControlType::MenuBar;
  widget = r_owner.info.menubar;
  owner  = &r_owner;
  show();
}

void MenuBar::finish() {
}

/*****
 * MenuGroup
 *****/

void MenuGroup::create(MenuBar &r_owner, const char *caption) {
  type   = ControlType::MenuGroup;
  widget = gtk_menu_new();
  item   = gtk_menu_item_new_with_label(caption ? caption : "?");
  owner  = r_owner.owner;
  parent = r_owner.widget;
}

void MenuGroup::create(MenuGroup &r_owner, const char *caption) {
  type   = ControlType::MenuGroup;
  widget = gtk_menu_new();
  item   = gtk_menu_item_new_with_label(caption ? caption : "?");
  owner  = r_owner.owner;
  parent = r_owner.widget;
}

void MenuGroup::finish() {
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), widget);
  gtk_menu_bar_append(parent, item);
  gtk_widget_show(item);
}

/*****
 * MenuItem
 *****/

void MenuItem::create(MenuGroup &r_owner, const char *caption) {
  type   = ControlType::MenuItem;
  widget = gtk_menu_item_new_with_label(caption ? caption : "?");
  owner  = r_owner.owner;
  parent = r_owner.widget;
  gtk_menu_shell_append(GTK_MENU_SHELL(parent), widget);
  g_signal_connect_swapped(G_OBJECT(widget), "activate",
    G_CALLBACK(libui_control_clicked), (gpointer)this);
  gtk_widget_show(widget);
}

/*****
 * MenuCheckItem
 *****/

void MenuCheckItem::create(MenuGroup &r_owner, const char *caption) {
  type   = ControlType::MenuCheckItem;
  widget = gtk_check_menu_item_new_with_label(caption ? caption : "?");
  owner  = r_owner.owner;
  parent = r_owner.widget;
  gtk_menu_shell_append(GTK_MENU_SHELL(parent), widget);
  g_signal_connect_swapped(G_OBJECT(widget), "activate",
    G_CALLBACK(libui_control_clicked), (gpointer)this);
  gtk_widget_show(widget);
}

void MenuCheckItem::check() {
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget), TRUE);
}

void MenuCheckItem::uncheck() {
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget), FALSE);
}

void MenuCheckItem::check(bool state) {
  (state == true) ? check() : uncheck();
}

bool MenuCheckItem::checked() {
  return gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));
}

/*****
 * MenuRadioItem
 *****/

void MenuRadioItem::create(MenuGroup &r_owner, ControlGroup &list, const char *caption) {
  if(list.count() == 0)throw;
  type   = ControlType::MenuRadioItem;
  widget = (&list[0] == this) ?
    gtk_radio_menu_item_new_with_label(0, caption ? caption : "?") :
    gtk_radio_menu_item_new_with_label_from_widget(GTK_RADIO_MENU_ITEM(list[0].widget), caption ? caption : "");
  owner  = r_owner.owner;
  parent = r_owner.widget;
  gtk_menu_shell_append(GTK_MENU_SHELL(parent), widget);
  g_signal_connect_swapped(G_OBJECT(widget), "activate",
    G_CALLBACK(libui_control_clicked), (gpointer)this);
  gtk_widget_show(widget);
}

void MenuRadioItem::check() {
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget), TRUE);
}

bool MenuRadioItem::checked() {
  return gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));
}

/*****
 * MenuSeparator
 *****/

void MenuSeparator::create(MenuGroup &r_owner) {
  type   = ControlType::MenuSeparator;
  widget = gtk_separator_menu_item_new();
  owner  = r_owner.owner;
  parent = r_owner.widget;
  gtk_menu_shell_append(GTK_MENU_SHELL(parent), widget);
  gtk_widget_show(widget);
}

/*****
 * Panel
 *****/

void Panel::create(Window &r_owner, uint style, uint x, uint y, uint width, uint height) {
  type   = ControlType::Panel;
  widget = gtk_fixed_new();
  owner  = &r_owner;
  gtk_widget_set_size_request(widget, width, height);
  gtk_fixed_put(GTK_FIXED(owner->info.container), widget, x, y);
  gtk_widget_show(widget);
}

void Panel::attach(Window &window) {
  if(attached) { //detach existing child window, return to toplevel window
    gtk_widget_reparent(attached->info.vcontainer, attached->info.window);
  }
  attached = &window;
  window.hide();
  gtk_widget_reparent(window.info.vcontainer, widget);
}

void Panel::detach() {
  if(attached) {
    gtk_widget_reparent(attached->info.vcontainer, attached->info.window);
  }
  attached = 0;
}

/*****
 * Container
 *****/

void Container::create(Window &r_owner, uint style, uint x, uint y, uint width, uint height) {
  type   = ControlType::Container;
  widget = gtk_drawing_area_new();
  owner  = &r_owner;
  gtk_widget_set_size_request(widget, width, height);
  gtk_fixed_put(GTK_FIXED(owner->info.container), widget, x, y);
  gtk_widget_show(widget);
}

void Container::set_background_color(uint8 r, uint8 g, uint8 b) {
GdkColor color;
  color.pixel = (r << 16) | (g << 8) | (b);
  color.red   = (r << 8) | (r);
  color.green = (g << 8) | (g);
  color.blue  = (b << 8) | (b);
  gtk_widget_modify_bg(widget, GTK_STATE_NORMAL, &color);
}

unsigned long Container::x_handle() {
  return GDK_WINDOW_XWINDOW(widget->window);
}

GtkWidget *Container::handle() {
  return widget;
}

/*****
 * Canvas
 *
 * Note: for reasons that defy any notion of logic, the GTK+ developers decided to
 * store color in semi-reversed format (XBGR) rather than conventional format (XRGB).
 * This is not an endian issue.
 * As a result, we are forced to perform manual conversion to XBGR format, at the
 * cost of significant overhead.
 *****/

void libui_canvas_expose(GtkWidget *widget, GdkEventAny *any, Canvas *canvas) {
uint32 *d = canvas->rbuffer;
uint32 *s = canvas->buffer;
  for(uint y = widget->allocation.height; y; y--) {
    for(uint x = widget->allocation.width; x; x--) {
    uint32 p = *s++;
      *d++ = ((p << 16) & 0xff0000) + (p & 0x00ff00) + ((p >> 16) & 0x0000ff);
    }
  }

  gdk_draw_rgb_32_image(widget->window,
    widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
    0, 0, widget->allocation.width, widget->allocation.height,
    GDK_RGB_DITHER_NONE, (guchar*)canvas->rbuffer, canvas->pitch);
}

void Canvas::create(Window &r_owner, uint style, uint x, uint y, uint width, uint height) {
  type   = ControlType::Canvas;
  widget = gtk_drawing_area_new();
  owner  = &r_owner;
  gtk_widget_set_size_request(widget, width, height);
  gtk_fixed_put(GTK_FIXED(owner->info.container), widget, x, y);
  gtk_widget_show(widget);

  pitch = width * sizeof(uint32);
  rbuffer = (uint32*)malloc(pitch * height);
  buffer  = (uint32*)malloc(pitch * height);
  memset(buffer,  0, pitch * height);
  memset(rbuffer, 0, pitch * height);
  g_signal_connect(G_OBJECT(widget), "expose_event", G_CALLBACK(libui_canvas_expose), this);
}

void Canvas::redraw() {
  if(!widget->window)return;

GdkRectangle rect;
  rect.x      = 0;
  rect.y      = 0;
  rect.width  = widget->allocation.width;
  rect.height = widget->allocation.height;
  gdk_window_invalidate_rect(widget->window, &rect, true);
}

Canvas::Canvas() {
  buffer = 0;
}

Canvas::~Canvas() {
  safe_free(rbuffer);
  safe_free(buffer);
}

/*****
 * Frame
 *****/

void Frame::create(Window &r_owner, uint style, uint x, uint y, uint width, uint height, const char *caption) {
  type   = ControlType::Frame;
  widget = gtk_frame_new(caption ? caption : "");
  owner  = &r_owner;
  gtk_widget_set_size_request(widget, width, height);
  gtk_fixed_put(GTK_FIXED(owner->info.container), widget, x, y);
  gtk_widget_show(widget);
}

/*****
 * Label
 *****/

void Label::create(Window &r_owner, uint style, uint x, uint y, uint width, uint height, const char *caption) {
  type   = ControlType::Label;
  widget = gtk_label_new(caption ? caption : "");
  owner  = &r_owner;
  gtk_misc_set_alignment(GTK_MISC(widget), 0.0, 0.0);
  gtk_widget_set_size_request(widget, width, height);
  gtk_fixed_put(GTK_FIXED(owner->info.container), widget, x, y);
  gtk_widget_show(widget);
}

void Label::set_text(const char *str) {
  gtk_label_set_label(GTK_LABEL(widget), str);
}

/*****
 * Button
 *****/

void Button::create(Window &r_owner, uint style, uint x, uint y, uint width, uint height, const char *caption) {
  type   = ControlType::Button;
  widget = gtk_button_new_with_label(caption ? caption : "");
  owner  = &r_owner;
  gtk_widget_set_size_request(widget, width, height);
  gtk_fixed_put(GTK_FIXED(owner->info.container), widget, x, y);
  gtk_widget_show(widget);

  g_signal_connect_swapped(G_OBJECT(widget), "clicked", G_CALLBACK(libui_control_clicked), (gpointer)this);
}

void Button::set_text(const char *str) {
  gtk_button_set_label(GTK_BUTTON(widget), str);
}

/*****
 * Checkbox
 *****/

void Checkbox::create(Window &r_owner, uint style, uint x, uint y, uint width, uint height, const char *caption) {
  type   = ControlType::Checkbox;
  widget = gtk_check_button_new_with_label(caption ? caption : "");
  owner  = &r_owner;
  gtk_widget_set_size_request(widget, width, height);
  gtk_fixed_put(GTK_FIXED(owner->info.container), widget, x, y);
  gtk_widget_show(widget);

  g_signal_connect_swapped(G_OBJECT(widget), "clicked", G_CALLBACK(libui_control_clicked), (gpointer)this);
}

void Checkbox::check() {
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), TRUE);
}

void Checkbox::uncheck() {
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), FALSE);
}

void Checkbox::check(bool state) {
  (state == true) ? check() : uncheck();
}

bool Checkbox::checked() {
  return (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)) == TRUE) ? true : false;
}

/*****
 * Radiobox
 *****/

void Radiobox::create(Window &r_owner, ControlGroup &group, uint style, uint x, uint y, uint width, uint height, const char *caption) {
  if(group.count() == 0)throw;
  type   = ControlType::Radiobox;
  widget = (&group[0] == this) ?
    gtk_radio_button_new_with_label(0, caption ? caption : "") :
    gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(group[0].widget), caption ? caption : "");
  owner  = &r_owner;
  gtk_widget_set_size_request(widget, width, height);
  gtk_fixed_put(GTK_FIXED(owner->info.container), widget, x, y);
  gtk_widget_show(widget);

  g_signal_connect_swapped(G_OBJECT(widget), "clicked", G_CALLBACK(libui_control_clicked), (gpointer)this);
}

void Radiobox::check() {
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), TRUE);
}

bool Radiobox::checked() {
  return (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)) == TRUE) ? true : false;
}

/*****
 * Editbox
 *****/

void Editbox::create(Window &r_owner, uint style, uint x, uint y, uint width, uint height, const char *caption) {
  owner = &r_owner;
  type = ControlType::Editbox;
  multiline = bool(style & Multiline);

  if(multiline == false) {
    widget = gtk_entry_new();
    if(style & Editbox::Readonly) { gtk_entry_set_editable(GTK_ENTRY(widget), false); }
    gtk_entry_set_text(GTK_ENTRY(widget), caption ? caption : "");
  } else {
  GtkPolicyType hscroll = (style & Editbox::HorizontalScrollAlways) ? GTK_POLICY_ALWAYS :
                 (style & Editbox::HorizontalScrollNever)  ? GTK_POLICY_NEVER  :
                 GTK_POLICY_AUTOMATIC;
  GtkPolicyType vscroll = (style & Editbox::VerticalScrollAlways) ? GTK_POLICY_ALWAYS :
                 (style & Editbox::VerticalScrollNever)  ? GTK_POLICY_NEVER  :
                 GTK_POLICY_AUTOMATIC;
    widget = gtk_scrolled_window_new(0, 0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(widget), hscroll, vscroll);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(widget), GTK_SHADOW_ETCHED_IN);
    subwidget = gtk_text_view_new();
    gtk_container_add(GTK_CONTAINER(widget), subwidget);
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(subwidget));
    if(style & Editbox::Readonly) { gtk_text_view_set_editable(GTK_TEXT_VIEW(subwidget), false); }
    gtk_text_buffer_set_text(buffer, caption ? caption : "", -1);
    gtk_widget_show(subwidget);
  }

  gtk_widget_set_size_request(widget, width, height);
  gtk_fixed_put(GTK_FIXED(owner->info.container), widget, x, y);
  gtk_widget_show(widget);
}

void Editbox::set_text(const char *str) {
  if(multiline == false) {
    gtk_entry_set_text(GTK_ENTRY(widget), str);
  } else {
    gtk_text_buffer_set_text(buffer, str, -1);
  }
}

uint Editbox::get_text(char *str, uint length) {
  if(multiline == false) {
  const char *temp = gtk_entry_get_text(GTK_ENTRY(widget));
    return strlcpy(str, temp ? temp : "", length);
  } else {
  GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);
    return strlcpy(str, gtk_text_buffer_get_text(buffer, &start, &end, true), length);
  }
}

/*****
 * Listbox
 *****/

/*****
 * GTK+'s implementation of list boxes was apparently someone's idea of a very, very cruel joke ...
 * Attempt to understand the below code at the risk of your own sanity.
 *****/

void Listbox::create(Window &r_owner, uint style, uint x, uint y, uint width, uint height, const char *columns) {
  owner = &r_owner;
  type = ControlType::Listbox;

bool header = bool(style & Header);

stringarray list, part;
  split(part, "|", columns);

GType *v = (GType*)malloc(count(part) * sizeof(GType));
  for(uint i = 0; i < count(part); i++) { v[i] = G_TYPE_STRING; }
  store = gtk_list_store_newv(count(part), v);
  safe_free(v);

  widget = gtk_scrolled_window_new(0, 0);
GtkPolicyType hscroll = (style & Listbox::HorizontalScrollAlways) ? GTK_POLICY_ALWAYS :
                        (style & Listbox::HorizontalScrollNever)  ? GTK_POLICY_NEVER  :
                        GTK_POLICY_AUTOMATIC;
GtkPolicyType vscroll = (style & Listbox::VerticalScrollAlways) ? GTK_POLICY_ALWAYS :
                        (style & Listbox::VerticalScrollNever)  ? GTK_POLICY_NEVER  :
                        GTK_POLICY_AUTOMATIC;
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(widget), hscroll, vscroll);
  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(widget), GTK_SHADOW_ETCHED_IN);
  subwidget = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
  gtk_container_add(GTK_CONTAINER(widget), subwidget);
  g_object_unref(G_OBJECT(store));
  gtk_widget_set_size_request(widget, width, height);
  gtk_fixed_put(GTK_FIXED(owner->info.container), widget, x, y);
  gtk_widget_show(subwidget);
  gtk_widget_show(widget);

  split(list, "|", columns);
//alternate colors for each listbox entry if there are multiple columns ...
  gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(subwidget), (count(list) >= 2) ? true : false);
  for(uint i = 0; i < count(list); i++) {
    renderer = gtk_cell_renderer_text_new();
    column   = gtk_tree_view_column_new_with_attributes(strptr(list[i]), renderer, "text", i, 0);
    column_list[column_list.size()] = column;
    gtk_tree_view_append_column(GTK_TREE_VIEW(subwidget), column);
  }

  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(subwidget), header);
  autosize_columns();

  g_signal_connect_swapped(G_OBJECT(subwidget), "cursor-changed", G_CALLBACK(libui_control_changed), (gpointer)this);
  g_signal_connect_swapped(G_OBJECT(subwidget), "row-activated", G_CALLBACK(libui_control_double_clicked), (gpointer)this);
}

void Listbox::autosize_columns() {
  gtk_tree_view_columns_autosize(GTK_TREE_VIEW(subwidget));
}

void Listbox::set_column_width(uint column, uint width) {
  gtk_tree_view_column_set_min_width(column_list[column], width);
  gtk_tree_view_column_set_max_width(column_list[column], width);
}

void Listbox::add_item(const char *data) {
stringarray part;
  split(part, "|", data);
  gtk_list_store_append(store, &iter);
  for(uint i = 0; i < count(part); i++) {
    gtk_list_store_set(store, &iter, i, strptr(part[i]), -1);
  }
}

void Listbox::set_item(uint index, const char *data) {
GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(subwidget));
  for(uint i = 0; i <= index; i++) {
    (i == 0) ?
      gtk_tree_model_get_iter_first(model, &iter) :
      gtk_tree_model_iter_next(model, &iter);
  }

stringarray part;
  split(part, "|", data);
  for(uint i = 0; i < count(part); i++) {
    gtk_list_store_set(store, &iter, i, strptr(part[i]), -1);
  }
}

//... because gtk_tree_view_get_selected_row(GTK_TREE_VIEW(subwidget)) would be too easy ...
int Listbox::get_selection() {
GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(subwidget));
GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(subwidget));
  if(gtk_tree_model_get_iter_first(model, &iter) == false) { return -1; }
  if(gtk_tree_selection_iter_is_selected(selection, &iter) == true) { return 0; }
  for(uint i = 1; i < 100000; i++) {
    if(gtk_tree_model_iter_next(model, &iter) == false) { return -1; }
    if(gtk_tree_selection_iter_is_selected(selection, &iter) == true) { return i; }
  }
  return -1;
}

//... because gtk_tree_view_set_selected_row(GTK_TREE_VIEW(subwidget), index) would be too easy ...
void Listbox::set_selection(int index) {
int current = get_selection();
GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(subwidget));
GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(subwidget));
  gtk_tree_selection_unselect_all(selection);
  if(index < 0) { goto end; }
  if(gtk_tree_model_get_iter_first(model, &iter) == false) { goto end; }
  if(index == 0) { gtk_tree_selection_select_iter(selection, &iter); goto end; }
  for(uint i = 1; i < 100000; i++) {
    if(gtk_tree_model_iter_next(model, &iter) == false) { goto end; }
    if(index == i) { gtk_tree_selection_select_iter(selection, &iter); goto end; }
  }
end:
  if(current != index) { owner->message(Message::Changed, (uintptr_t)this); }
}

void Listbox::reset() {
  gtk_list_store_clear(GTK_LIST_STORE(store));
  gtk_tree_view_set_model(GTK_TREE_VIEW(subwidget), GTK_TREE_MODEL(store));
}

/*****
 * Combobox
 *****/

void Combobox::create(Window &r_owner, uint style, uint x, uint y, uint width, uint height) {
  type    = ControlType::Combobox;
  widget  = gtk_combo_box_new_text(); //gtk_combo_box_entry_new_text(); /* alternate style */
  owner   = &r_owner;
  counter = 0;
  gtk_widget_set_size_request(widget, width, height);
  gtk_fixed_put(GTK_FIXED(owner->info.container), widget, x, y);
  gtk_widget_show(widget);
}

void Combobox::add_item(const char *data) {
  gtk_combo_box_append_text(GTK_COMBO_BOX(widget), data);
  if(counter++ == 0) { set_selection(0); }
}

int Combobox::get_selection() {
  return gtk_combo_box_get_active(GTK_COMBO_BOX(widget));
}

void Combobox::set_selection(int index) {
  gtk_combo_box_set_active(GTK_COMBO_BOX(widget), index);
}

void Combobox::reset() {
  if(counter == 0) { return; }
  for(int i = (counter - 1); i >= 0; i--) {
    gtk_combo_box_remove_text(GTK_COMBO_BOX(widget), i);
  }
  counter = 0;
}

/*****
 * Progressbar
 *****/

void Progressbar::create(Window &r_owner, uint style, uint x, uint y, uint width, uint height) {
  type   = ControlType::Progressbar;
  widget = gtk_progress_bar_new();
  owner  = &r_owner;
  gtk_widget_set_size_request(widget, width, height);
  gtk_fixed_put(GTK_FIXED(owner->info.container), widget, x, y);
  gtk_widget_show(widget);
}

void Progressbar::set_progress(uint progress) {
  progress = minmax<0, 100>(progress);
double p = (double)progress / 100.0;
  gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(widget), p);
}

uint Progressbar::get_progress() {
uint p = (uint)(gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(widget)) * 100.0);
  return (uint)minmax<0, 100>(p);
}

/*****
 * Slider
 *****/

void Slider::create(Window &r_owner, uint style, uint x, uint y, uint width, uint height, uint range) {
  type = ControlType::Slider;
  orientation = (style & Vertical) ? 1 : 0;

  if(range < 1)range = 1;
  if(orientation == 0) {
    widget = gtk_hscale_new_with_range(0, range - 1, 1);
  } else {
    widget = gtk_vscale_new_with_range(0, range - 1, 1);
  }

  owner = &r_owner;
  gtk_scale_set_draw_value(GTK_SCALE(widget), FALSE);
  gtk_widget_set_size_request(widget, width, height);
  gtk_fixed_put(GTK_FIXED(owner->info.container), widget, x, y);
  gtk_widget_show(widget);

  g_signal_connect_swapped(G_OBJECT(widget), "value-changed", G_CALLBACK(libui_control_changed), (gpointer)this);
}

void Slider::set_position(uint position) {
  gtk_range_set_value(GTK_RANGE(widget), position);
}

uint Slider::get_position() {
  return (int)gtk_range_get_value(GTK_RANGE(widget));
}

};
