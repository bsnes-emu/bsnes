int CheatEditorWindow::message(uint id, void *param) {
  return 0;
}

void CheatEditorWindow::setup() {
  create(0, 475, 355);

int x = 0, y = 0;
  list.create(*this, ui::Listbox::Header | ui::Listbox::VerticalScrollAlways, x, y, 475, 285, "Status|Code|Description");
  list.add_item("Enabled|0123-4567|Infinite Energy");
  list.add_item("Disabled|89ab-cdef|Infinite Lives");
  list.autosize_columns();
  y += 290;

  add_code.create   (*this, 0, x,       y, 155, 30, "Add Code");
  toggle_code.create(*this, 0, x + 160, y, 155, 30, "Toggle Status");
  delete_code.create(*this, 0, x + 320, y, 155, 30, "Delete Code");
  y += 35;

  code.create(*this, 0, x,       y, 155, 30, "<code>");
  desc.create(*this, 0, x + 160, y, 315, 30, "<description>");

  add_code.disable();
  toggle_code.disable();
  delete_code.disable();
}
