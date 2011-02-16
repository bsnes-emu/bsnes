void pLineEdit::setEditable(bool editable) {
}

void pLineEdit::setText(const string &text) {
}

string pLineEdit::text() {
}

pLineEdit::pLineEdit(LineEdit &lineEdit) : pWidget(lineEdit), lineEdit(lineEdit) {
}
