void pMenuItem::setText(const string &text) {
}

pMenuItem::pMenuItem(MenuItem &menuItem) : pAction(menuItem), menuItem(menuItem) {
}
