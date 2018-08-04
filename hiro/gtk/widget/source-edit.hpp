#if defined(Hiro_SourceEdit)

namespace hiro {

struct pSourceEdit : pWidget {
  Declare(SourceEdit, Widget)

  auto setCursor(Cursor cursor) -> void;
  auto setEditable(bool editable) -> void;
  auto setFocused() -> void override;
  auto setText(const string& text) -> void;
  auto setWordWrap(bool wordWrap) -> void;
  auto text() const -> string;

  GtkScrolledWindow* gtkScrolledWindow = nullptr;
  GtkContainer* gtkContainer = nullptr;
  GtkSourceBuffer* gtkSourceBuffer = nullptr;
  GtkTextBuffer* gtkTextBuffer = nullptr;
  GtkSourceLanguageManager* gtkSourceLanguageManager = nullptr;
  GtkSourceLanguage* gtkSourceLanguage = nullptr;
  GtkSourceStyleSchemeManager* gtkSourceStyleSchemeManager = nullptr;
  GtkSourceStyleScheme* gtkSourceStyleScheme = nullptr;
  GtkSourceView* gtkSourceView = nullptr;
  GtkTextView* gtkTextView = nullptr;
  GtkWidget* gtkWidgetSourceView = nullptr;
};

}

#endif
