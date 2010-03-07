#ifndef NALL_QT_HEXEDITOR_HPP
#define NALL_QT_HEXEDITOR_HPP

#include <nall/function.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>

namespace nall {

class HexEditor : public QTextEdit {
  Q_OBJECT

public:
  function<uint8_t (unsigned)> reader;
  function<void (unsigned, uint8_t)> writer;

  void setColumns(unsigned columns);
  void setRows(unsigned rows);
  void setOffset(unsigned offset);
  void setSize(unsigned size);
  unsigned lineWidth() const;
  void refresh();

  HexEditor();

protected slots:
  void scrolled();

protected:
  QHBoxLayout *layout;
  QScrollBar *scrollBar;
  unsigned editorColumns;
  unsigned editorRows;
  unsigned editorOffset;
  unsigned editorSize;
  bool lock;

  void keyPressEvent(QKeyEvent*);
};

inline void HexEditor::keyPressEvent(QKeyEvent *event) {
  QTextCursor cursor = textCursor();
  unsigned x = cursor.position() % lineWidth();
  unsigned y = cursor.position() / lineWidth();

  int hexCode = -1;
  switch(event->key()) {
    case Qt::Key_0: hexCode =  0; break;
    case Qt::Key_1: hexCode =  1; break;
    case Qt::Key_2: hexCode =  2; break;
    case Qt::Key_3: hexCode =  3; break;
    case Qt::Key_4: hexCode =  4; break;
    case Qt::Key_5: hexCode =  5; break;
    case Qt::Key_6: hexCode =  6; break;
    case Qt::Key_7: hexCode =  7; break;
    case Qt::Key_8: hexCode =  8; break;
    case Qt::Key_9: hexCode =  9; break;
    case Qt::Key_A: hexCode = 10; break;
    case Qt::Key_B: hexCode = 11; break;
    case Qt::Key_C: hexCode = 12; break;
    case Qt::Key_D: hexCode = 13; break;
    case Qt::Key_E: hexCode = 14; break;
    case Qt::Key_F: hexCode = 15; break;
  }

  if(cursor.hasSelection() == false && hexCode != -1) {
    bool cursorOffsetValid = (x >= 11 && ((x - 11) % 3) != 2);
    if(cursorOffsetValid) {
      bool nibble = (x - 11) % 3;  //0 = top nibble, 1 = bottom nibble
      unsigned cursorOffset = y * editorColumns + ((x - 11) / 3);
      unsigned effectiveOffset = editorOffset + cursorOffset;
      if(effectiveOffset >= editorSize) effectiveOffset %= editorSize;

      uint8_t data = reader ? reader(effectiveOffset) : 0x00;
      data &= (nibble == 0 ? 0x0f : 0xf0);
      data |= (nibble == 0 ? (hexCode << 4) : (hexCode << 0));
      if(writer) writer(effectiveOffset, data);
      refresh();

      cursor.setPosition(y * lineWidth() + x + 1);  //advance cursor
      setTextCursor(cursor);
    }
  } else {
    //allow navigation keys to move cursor, but block text input
    setTextInteractionFlags(Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse);
    QTextEdit::keyPressEvent(event);
    setTextInteractionFlags(Qt::TextEditorInteraction);
  }
}

inline void HexEditor::setColumns(unsigned columns) {
  editorColumns = columns;
}

inline void HexEditor::setRows(unsigned rows) {
  editorRows = rows;
  scrollBar->setPageStep(editorRows);
}

inline void HexEditor::setOffset(unsigned offset) {
  lock = true;
  editorOffset = offset;
  scrollBar->setSliderPosition(editorOffset / editorColumns);
  lock = false;
}

inline void HexEditor::setSize(unsigned size) {
  editorSize = size;
  bool indivisible = (editorSize % editorColumns) != 0;  //add one for incomplete row
  scrollBar->setRange(0, editorSize / editorColumns + indivisible - editorRows);
}

inline unsigned HexEditor::lineWidth() const {
  return 11 + 3 * editorColumns;
}

inline void HexEditor::refresh() {
  string output;
  char temp[256];
  unsigned offset = editorOffset;

  for(unsigned y = 0; y < editorRows; y++) {
    if(offset >= editorSize) break;
    sprintf(temp, "%.4x:%.4x", (offset >> 16) & 0xffff, (offset >> 0) & 0xffff);
    output << "<font color='#808080'>" << temp << "</font>&nbsp;&nbsp;";

    for(unsigned x = 0; x < editorColumns; x++) {
      if(offset >= editorSize) break;
      sprintf(temp, "%.2x", reader ? reader(offset) : 0x00);
      offset++;
      output << "<font color='" << ((x & 1) ? "#000080" : "#0000ff") << "'>" << temp << "</font>";
      if(x != (editorColumns - 1)) output << "&nbsp;";
    }

    if(y != (editorRows - 1)) output << "<br>";
  }

  setHtml(output);
}

inline void HexEditor::scrolled() {
  if(lock) return;
  unsigned offset = scrollBar->sliderPosition();
  editorOffset = offset * editorColumns;
  refresh();
}

inline HexEditor::HexEditor() {
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  layout = new QHBoxLayout;
  layout->setAlignment(Qt::AlignRight);
  layout->setMargin(0);
  layout->setSpacing(0);
  setLayout(layout);

  scrollBar = new QScrollBar(Qt::Vertical);
  scrollBar->setSingleStep(1);
  layout->addWidget(scrollBar);

  lock = false;
  connect(scrollBar, SIGNAL(actionTriggered(int)), this, SLOT(scrolled()));

  setColumns(16);
  setRows(16);
  setSize(0);
  setOffset(0);
}

}

#endif
