class QbHexEditScrollBar : public QScrollBar {
  Q_OBJECT

public:
};

class QbHexEdit : public QTextEdit {
  Q_OBJECT

public:
  function<uint8 (unsigned)> reader;
  function<void (unsigned, uint8)> writer;

  void keyPressEvent(QKeyEvent*);

  void setDocumentSize(unsigned);
  void setDocumentOffset(unsigned);

  void setColumns(unsigned);
  void setRows(unsigned);

  void update();
  QbHexEdit();

private:
  QbHexEditScrollBar *scrollBar;

  unsigned documentSize;
  unsigned documentOffset;

  unsigned editorColumns;
  unsigned editorRows;
  unsigned editorLineWidth;
};
