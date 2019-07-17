#if defined(Hiro_MenuSeparator)

namespace hiro {

auto pMenuSeparator::construct() -> void {
  widget = gtk_separator_menu_item_new();
}

auto pMenuSeparator::destruct() -> void {
  if(widget) gtk_widget_destroy(widget), widget = nullptr;
}

}

#endif
