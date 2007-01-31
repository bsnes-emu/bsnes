namespace libui {

/*****
 * FileLoad
 *****/

bool Window::file_load(char *filename, const char *filter, const char *path) {
  strcpy(filename, "");

GtkWidget *dialog = gtk_file_chooser_dialog_new("Load File",
  GTK_WINDOW(info.window), GTK_FILE_CHOOSER_ACTION_OPEN,
  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
  GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, 0);
  if(path && strcmp(path, "")) {
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), path);
  }

  if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
  char *fn = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    strcpy(filename, fn);
    g_free(fn);
  }

  gtk_widget_destroy(dialog);
  return strcmp(filename, ""); //return true if filename != ""
}

/*****
 * FileSave
 *****/

bool Window::file_save(char *filename, const char *filter, const char *path) {
  strcpy(filename, "");

GtkWidget *dialog = gtk_file_chooser_dialog_new("Save File",
  GTK_WINDOW(info.window), GTK_FILE_CHOOSER_ACTION_SAVE,
  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
  GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, 0);
  if(path && strcmp(path, "")) {
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), path);
  }
  gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);

  if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
  char *fn = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    strcpy(filename, fn);
    g_free(fn);
  }

  gtk_widget_destroy(dialog);
  return strcmp(filename, ""); //return true if filename != ""
}

/*****
 * Control
 *****/

void Control::move(uint x, uint y) {
  gtk_fixed_move(GTK_FIXED(owner->info.container), widget, x, y);
}

void Control::resize(uint width, uint height) {
  gtk_widget_set_size_request(widget, width, height);
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
 * Menu
 *****/

void Window::menu_begin() {}
void Window::menu_end() {}

/*****
 * Menu
 *****/

void Window::menu_group_begin(MenuGroup &group) {
  info.control[info.control_index] = &group;
  group.id     = info.control_index++;
  group.owner  = this;
  info.menu_group[++info.menu_group_index] = &group;
}

void Window::menu_group_end() {
MenuGroup *group = info.menu_group[info.menu_group_index--];
MenuGroup *owner = info.menu_group[info.menu_group_index];
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(group->item), group->widget);
  gtk_menu_bar_append(owner->widget, group->item);
  gtk_widget_show(group->item);
}

void Window::menu_add_item(Control &item) {
  info.control[info.control_index] = &item;
MenuGroup &group = *info.menu_group[info.menu_group_index];
  item.id     = info.control_index++;
  item.owner  = this;
  gtk_menu_shell_append(GTK_MENU_SHELL(group.widget), item.widget);
  g_signal_connect_swapped(G_OBJECT(item.widget), "activate",
    G_CALLBACK(libui_control_clicked), (gpointer)&item);
}

void Window::menu_add_separator() {
MenuItem *item = new MenuItem();
  info.control[info.control_index] = item;
MenuGroup *group = info.menu_group[info.menu_group_index];
  item->id     = info.control_index++;
  item->owner  = this;
  item->type   = Control::MenuItem;
  item->widget = gtk_separator_menu_item_new();
  gtk_menu_shell_append(GTK_MENU_SHELL(group->widget), item->widget);
  gtk_widget_show(item->widget);
}

/*****
 * MenuGroup
 *****/

MenuGroup& MenuGroup::create(const char *caption) {
  type   = Control::MenuGroup;
  widget = gtk_menu_new();
  item   = gtk_menu_item_new_with_label(caption ? caption : "?");
  return *this;
}

/*****
 * MenuItem
 *****/

MenuItem& MenuItem::create(const char *caption) {
  type   = Control::MenuItem;
  widget = gtk_menu_item_new_with_label(caption ? caption : "?");
  gtk_widget_show(widget);
  return *this;
}

/*****
 * MenuCheckItem
 *****/

MenuCheckItem& MenuCheckItem::create(const char *caption) {
  type   = Control::MenuCheckItem;
  widget = gtk_check_menu_item_new_with_label(caption ? caption : "?");
  gtk_widget_show(widget);
  return *this;
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

MenuRadioItem& MenuRadioItem::create(ControlGroup &list, const char *caption) {
  if(list.count() == 0)throw;
  type   = Control::MenuRadioItem;
  widget = (&list[0] == this) ?
    gtk_radio_menu_item_new_with_label(0, caption ? caption : "?") :
    gtk_radio_menu_item_new_with_label_from_widget(GTK_RADIO_MENU_ITEM(list[0].widget), caption ? caption : "");
  gtk_widget_show(widget);
  return *this;
}

void MenuRadioItem::check() {
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget), TRUE);
}

bool MenuRadioItem::checked() {
  return gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));
}

/*****
 * Container
 *****/

Container &Container::create(const char *style, uint width, uint height) {
  type   = Control::Container;
  widget = gtk_fixed_new();
  gtk_widget_set_size_request(widget, width, height);
  gtk_widget_show(widget);
  return *this;
}

void Container::attach(Control &control, uint x, uint y) {
  owner->attach(control, x, y, false);
  gtk_fixed_put(GTK_FIXED(widget), control.widget, x, y);
}

/*****
 * Canvas
 *****/

Canvas &Canvas::create(const char *style, uint width, uint height) {
  type   = Control::Canvas;
  widget = gtk_drawing_area_new();
  gtk_widget_set_size_request(widget, width, height);
  gtk_widget_show(widget);
  return *this;
}

WindowHandle Canvas::handle() {
  return (WindowHandle)(GDK_WINDOW_XWINDOW(widget->window));
}

void Canvas::set_background_color(uint8 r, uint8 g, uint8 b) {
GdkColor color;
  color.pixel = (r << 16) | (g << 8) | (b);
  color.red   = (r << 8) | (r);
  color.green = (g << 8) | (g);
  color.blue  = (b << 8) | (b);
  gtk_widget_modify_bg(widget, GTK_STATE_NORMAL, &color);
}

/*****
 * Frame
 *****/

Frame& Frame::create(const char *style, uint width, uint height, const char *caption) {
  type   = Control::Frame;
  widget = gtk_frame_new(caption ? caption : "");
  gtk_widget_set_size_request(widget, width, height);
  gtk_widget_show(widget);
  return *this;
}

/*****
 * Label
 *****/

Label& Label::create(const char *style, uint width, uint height, const char *caption) {
  type   = Control::Label;
  widget = gtk_label_new(caption ? caption : "");
  gtk_misc_set_alignment(GTK_MISC(widget), 0.0, 0.0);
  gtk_widget_set_size_request(widget, width, height);
  gtk_widget_show(widget);
  return *this;
}

void Label::set_text(const char *str, ...) {
va_list args;
  va_start(args, str);
string temp;
  vsprintf(temp, str, args);
  va_end(args);
  gtk_label_set_label(GTK_LABEL(widget), strptr(temp));
}

uint Label::get_text(char *str, uint length) {
const char *temp = gtk_label_get_text(GTK_LABEL(widget));
  return strlcpy(str, temp ? temp : "", length);
}

/*****
 * Button
 *****/

Button& Button::create(const char *style, uint width, uint height, const char *caption) {
  type   = Control::Button;
  widget = gtk_button_new_with_label(caption ? caption : "");
  gtk_widget_set_size_request(widget, width, height);
  gtk_widget_show(widget);
  return *this;
}

void Button::set_text(const char *str, ...) {
va_list args;
  va_start(args, str);
string temp;
  vsprintf(temp, str, args);
  va_end(args);
  gtk_button_set_label(GTK_BUTTON(widget), strptr(temp));
}

uint Button::get_text(char *str, uint length) {
const char *temp = gtk_button_get_label(GTK_BUTTON(widget));
  return strlcpy(str, temp ? temp : "", length);
}

/*****
 * Checkbox
 *****/

Checkbox& Checkbox::create(const char *style, uint width, uint height, const char *caption) {
  type   = Control::Checkbox;
  widget = gtk_check_button_new_with_label(caption ? caption : "");
  gtk_widget_set_size_request(widget, width, height);
  gtk_widget_show(widget);
  return *this;
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

Radiobox& Radiobox::create(ControlGroup &group, const char *style, uint width, uint height, const char *caption) {
  if(group.count() == 0)throw;
  type   = Control::Radiobox;
  widget = (&group[0] == this) ?
    gtk_radio_button_new_with_label(0, caption ? caption : "") :
    gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(group[0].widget), caption ? caption : "");
  gtk_widget_set_size_request(widget, width, height);
  gtk_widget_show(widget);
  return *this;
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

Editbox& Editbox::create(const char *style, uint width, uint height, const char *caption) {
  type = Control::Editbox;
  multiline = false;
stringarray part;
  split(part, "|", style);
  for(uint i = 0; i < count(part); i++) {
    if(part[i] == "multiline")multiline = true;
  }

  if(multiline == false) {
    widget = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(widget), caption ? caption : "");
  } else {
    widget = gtk_text_view_new();
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));
    gtk_text_buffer_set_text(buffer, caption ? caption : "", -1);
  }

  gtk_widget_set_size_request(widget, width, height);
  gtk_widget_show(widget);
  return *this;
}

void Editbox::set_text(const char *str, ...) {
va_list args;
  va_start(args, str);
string temp;
  vsprintf(temp, str, args);
  va_end(args);

  if(multiline == false) {
    gtk_entry_set_text(GTK_ENTRY(widget), strptr(temp));
  } else {
    gtk_text_buffer_set_text(buffer, strptr(temp), -1);
  }
}

uint Editbox::get_text(char *str, uint length) {
  if(multiline == false) {
  const char *temp = gtk_entry_get_text(GTK_ENTRY(widget));
    return strlcpy(str, temp ? temp : "", length);
  } else {
  //not sure how to use GtkTextIter* to retrieve editbox text buffer ...
    strcpy(str, "");
    return 0;
  }
}

/*****
 * Listbox
 *****/

/*****
 * GTK+'s implementation of list boxes was apparently someone's idea of a very, very cruel joke ...
 * Attempt to understand the below code at the risk of your own sanity.
 *****/

Listbox& Listbox::create(const char *style, uint width, uint height, const char *columns, const char *data) {
  type = Control::Listbox;
stringarray list, part;
  split(part, "|", style);
bool header = false;
  for(uint i = 0; i < count(part); i++) {
    if(part[i] == "header")header = true;
  }

  split(part, "|", columns);

GType *v = (GType*)malloc(count(part) * sizeof(GType));
  for(uint i = 0; i < count(part); i++) { v[i] = G_TYPE_STRING; }
  store = gtk_list_store_newv(count(part), v);
  safe_free(v);

  if(data && strcmp(data, "")) {
    split(list, "||", data);
    for(uint l = 0; l < count(list); l++) {
      gtk_list_store_append(store, &iter);
      split(part, "|", list[l]);
      for(uint i = 0; i < count(part); i++) {
        gtk_list_store_set(store, &iter, i, strptr(part[i]), -1);
      }
    }
  }

  widget = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
  g_object_unref(G_OBJECT(store));
  gtk_widget_set_size_request(widget, width, height);
  gtk_widget_show(widget);

  split(list, "|", columns);
  for(uint i = 0; i < count(list); i++) {
    renderer = gtk_cell_renderer_text_new();
    column   = gtk_tree_view_column_new_with_attributes(strptr(list[i]), renderer, "text", i, 0);
    column_list[column_list.size()] = column;
    gtk_tree_view_append_column(GTK_TREE_VIEW(widget), column);
  }

  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(widget), header);
  autosize_columns();
  return *this;
}

void Listbox::autosize_columns() {
  gtk_tree_view_columns_autosize(GTK_TREE_VIEW(widget));
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

int Listbox::get_selection() {
//... because gtk_tree_view_get_selected_row(GTK_TREE_VIEW(widget)) would be too easy ...
GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(widget));
  if(gtk_tree_model_get_iter_first(model, &iter) == false) { return -1; }
  if(gtk_tree_selection_iter_is_selected(selection, &iter) == true) { return 0; }
  for(uint i = 1; i < 100000; i++) {
    if(gtk_tree_model_iter_next(model, &iter) == false) { return -1; }
    if(gtk_tree_selection_iter_is_selected(selection, &iter) == true) { return i; }
  }
  return -1;
}

void Listbox::set_selection(int index) {
//... because gtk_tree_view_set_selected_row(GTK_TREE_VIEW(widget), index) would be too easy ...
GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(widget));
  gtk_tree_selection_unselect_all(selection);
  if(index < 0) { return; }
  if(gtk_tree_model_get_iter_first(model, &iter) == false) { return; }
  if(index == 0) { gtk_tree_selection_select_iter(selection, &iter); return; }
  for(uint i = 1; i < 100000; i++) {
    if(gtk_tree_model_iter_next(model, &iter) == false) { return; }
    if(index == i) { gtk_tree_selection_select_iter(selection, &iter); return; }
  }
}

void Listbox::reset() {
  gtk_list_store_clear(GTK_LIST_STORE(store));
  gtk_tree_view_set_model(GTK_TREE_VIEW(widget), GTK_TREE_MODEL(store));
}

/*****
 * Combobox
 *****/

Combobox& Combobox::create(const char *style, uint width, uint height, const char *data) {
  type    = Control::Combobox;
  widget  = gtk_combo_box_new_text(); //gtk_combo_box_entry_new_text();
  counter = 0;
  gtk_widget_set_size_request(widget, width, height);
  gtk_widget_show(widget);

  if(data && strcmp(data, "")) {
  stringarray temp;
    split(temp, "|", data);
    for(int i = 0; i < count(temp); i++) {
      gtk_combo_box_append_text(GTK_COMBO_BOX(widget), strptr(temp[i]));
      counter++;
    }
    set_selection(0);
  }

  return *this;
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

Progressbar& Progressbar::create(const char *style, uint width, uint height) {
  type   = Control::Progressbar;
  widget = gtk_progress_bar_new();
  gtk_widget_set_size_request(widget, width, height);
  gtk_widget_show(widget);
  return *this;
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

Slider& Slider::create(const char *style, uint width, uint height, uint min, uint max) {
  type = Control::Slider;
  orientation = 0;

stringarray part;
  split(part, "|", style);
  for(uint i = 0; i < count(part); i++) {
    if(part[i] == "horizontal")orientation = 0;
    if(part[i] == "vertical")orientation = 1;
  }

  if(orientation == 0) {
    widget = gtk_hscale_new_with_range(min, max, 1);
  } else {
    widget = gtk_vscale_new_with_range(min, max, 1);
  }

  gtk_scale_set_draw_value(GTK_SCALE(widget), FALSE);
  gtk_widget_set_size_request(widget, width, height);
  gtk_widget_show(widget);
  return *this;
}

void Slider::set_position(uint position) {
  gtk_range_set_value(GTK_RANGE(widget), position);
}

uint Slider::get_position() {
  return (int)gtk_range_get_value(GTK_RANGE(widget));
}

};
