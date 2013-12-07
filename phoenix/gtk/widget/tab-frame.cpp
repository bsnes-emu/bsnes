namespace phoenix {

static void TabFrame_change(GtkNotebook* notebook, GtkWidget* page, unsigned selection, TabFrame* self) {
  self->state.selection = selection;
  self->p.synchronizeLayout();
  if(!self->p.locked && self->onChange) self->onChange();
}

void pTabFrame::append(string text, const image& image) {
  unsigned selection = tabFrame.state.text.size() - 1;

  Tab tab;
  tab.child = gtk_fixed_new();
  tab.container = gtk_hbox_new(false, 0);
  tab.image = gtk_image_new();
  tab.title = gtk_label_new(text);
  tabs.append(tab);

  gtk_widget_show(tab.child);
  gtk_widget_show(tab.container);
  gtk_widget_show(tab.image);
  gtk_widget_show(tab.title);
  gtk_box_pack_start(GTK_BOX(tab.container), tab.image, false, false, 0);
  gtk_box_pack_start(GTK_BOX(tab.container), tab.title, false, false, 2);

  gtk_notebook_append_page(GTK_NOTEBOOK(gtkWidget), tab.child, tab.container);
  setFont(widget.state.font);
  if(!image.empty()) setImage(selection, image);
}

GtkWidget* pTabFrame::container(Widget& widget) {
  Layout* widgetLayout = GetParentWidgetLayout(&widget);
  unsigned selection = 0;
  for(auto& layout : tabFrame.state.layout) {
    if(layout == widgetLayout) return tabs[selection].child;
    selection++;
  }
  return nullptr;
}

Position pTabFrame::displacement() {
  return {6, 31};
}

void pTabFrame::remove(unsigned selection) {
  tabs.remove(selection);
  gtk_notebook_remove_page(GTK_NOTEBOOK(gtkWidget), selection);
}

void pTabFrame::setEnabled(bool enabled) {
  for(auto& layout : tabFrame.state.layout) {
    if(layout) layout->setEnabled(layout->enabled());
  }
  pWidget::setEnabled(enabled);
}

void pTabFrame::setGeometry(Geometry geometry) {
  pWidget::setGeometry(geometry);
  geometry.x += 1, geometry.width -= 5;
  geometry.y += 26, geometry.height -= 31;
  for(auto& layout : tabFrame.state.layout) {
    if(layout) layout->setGeometry(geometry);
  }
  synchronizeLayout();
}

void pTabFrame::setImage(unsigned selection, const image& image) {
  if(image.empty() == false) {
    nall::image copy = image;
    unsigned size = pFont::size(widget.state.font, " ").height;
    copy.scale(size, size);
    GdkPixbuf* pixbuf = CreatePixbuf(copy);
    gtk_image_set_from_pixbuf(GTK_IMAGE(tabs[selection].image), pixbuf);
  } else {
    gtk_image_clear(GTK_IMAGE(tabs[selection].image));
  }
}

void pTabFrame::setSelection(unsigned selection) {
  locked = true;
  gtk_notebook_set_current_page(GTK_NOTEBOOK(gtkWidget), selection);
  locked = false;
}

void pTabFrame::setText(unsigned selection, string text) {
  gtk_label_set_text(GTK_LABEL(tabs[selection].title), text);
}

void pTabFrame::setVisible(bool visible) {
  for(auto& layout : tabFrame.state.layout) {
    if(layout) layout->setVisible(layout->visible());
  }
  pWidget::setVisible(visible);
}

void pTabFrame::constructor() {
  gtkWidget = gtk_notebook_new();
  gtk_notebook_set_show_border(GTK_NOTEBOOK(gtkWidget), false);
  gtk_notebook_set_tab_pos(GTK_NOTEBOOK(gtkWidget), GTK_POS_TOP);
  g_signal_connect(G_OBJECT(gtkWidget), "switch-page", G_CALLBACK(TabFrame_change), (gpointer)&tabFrame);

  setSelection(tabFrame.state.selection);
}

void pTabFrame::destructor() {
  gtk_widget_destroy(gtkWidget);
}

void pTabFrame::orphan() {
  destructor();
  constructor();
}

void pTabFrame::setFont(string font) {
  for(auto& tab : tabs) pFont::setFont(tab.title, font);
}

void pTabFrame::synchronizeLayout() {
  unsigned selection = 0;
  for(auto& layout : tabFrame.state.layout) {
    if(layout) layout->setVisible(selection == tabFrame.state.selection);
    selection++;
  }
}

}
