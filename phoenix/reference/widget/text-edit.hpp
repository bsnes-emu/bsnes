namespace phoenix {

struct pTextEdit : public pWidget {
  TextEdit& textEdit;

  void setBackgroundColor(Color color);
  void setCursorPosition(unsigned position);
  void setEditable(bool editable);
  void setForegroundColor(Color color);
  void setText(string text);
  void setWordWrap(bool wordWrap);
  string text();

  pTextEdit(TextEdit& textEdit) : pWidget(textEdit), textEdit(textEdit) {}
  void constructor();
  void destructor();
};

}
