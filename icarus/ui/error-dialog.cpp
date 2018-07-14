ErrorDialog::ErrorDialog() {
  errorDialog = this;

  onClose([&] { setVisible(false); scanDialog->show(); });
  layout.setPadding(5);
  errorLog.setEditable(false);
  closeButton.setText("Close").onActivate([&] { doClose(); });

  setSize({800, 360});
  setCentered();
}

auto ErrorDialog::show(const string& text) -> void {
  errorLog.setText(text);
  setVisible();
}
