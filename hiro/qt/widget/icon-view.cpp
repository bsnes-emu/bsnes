#if defined(Hiro_IconView)

namespace hiro {

void pIconView::append() {
  lock();
  auto item = new QListWidgetItem(qtIconView);
  unlock();
}

void pIconView::remove(unsigned selection) {
  lock();
  if(auto item = qtIconView->item(selection)) {
    delete item;
  }
  unlock();
}

void pIconView::reset() {
  lock();
  qtIconView->clear();
  unlock();
}

void pIconView::setBackgroundColor(Color color) {
  QPalette palette = qtIconView->palette();
  palette.setColor(QPalette::Base, QColor(color.red, color.green, color.blue));
  qtIconView->setPalette(palette);
  qtIconView->setAutoFillBackground(true);
}

void pIconView::setFlow(Orientation flow) {
  qtIconView->setFlow(flow == Orientation::Horizontal ? QListView::LeftToRight : QListView::TopToBottom);
  qtIconView->resize(qtIconView->size());  //adjust visibility of scroll bars
}

void pIconView::setForegroundColor(Color color) {
  QPalette palette = qtIconView->palette();
  palette.setColor(QPalette::Text, QColor(color.red, color.green, color.blue));
  qtIconView->setPalette(palette);
}

void pIconView::setImage(unsigned selection, const image& image) {
  if(auto item = qtIconView->item(selection)) {
    item->setIcon(CreateIcon(image));
  }
}

void pIconView::setOrientation(Orientation orientation) {
  qtIconView->setViewMode(orientation == Orientation::Horizontal ? QListView::ListMode : QListView::IconMode);
  qtIconView->setWrapping(true);
}

void pIconView::setSelected(unsigned selection, bool selected) {
  lock();
  if(auto item = qtIconView->item(selection)) {
    item->setSelected(selected);
  }
  unlock();
}

void pIconView::setSelected(const vector<unsigned>& selections) {
  lock();
  qtIconView->clearSelection();
  for(auto& selection : selections) {
    if(auto item = qtIconView->item(selection)) {
      item->setSelected(true);
    }
  }
  unlock();
}

void pIconView::setSelectedAll() {
  lock();
  qtIconView->selectAll();
  unlock();
}

void pIconView::setSelectedNone() {
  lock();
  qtIconView->clearSelection();
  unlock();
}

void pIconView::setSingleSelection(bool singleSelection) {
  qtIconView->setSelectionMode(singleSelection ? QAbstractItemView::SingleSelection : QAbstractItemView::ExtendedSelection);
}

void pIconView::setText(unsigned selection, const string& text) {
  if(auto item = qtIconView->item(selection)) {
    item->setText(QString::fromUtf8(text));
  }
}

void pIconView::constructor() {
  qtWidget = qtIconView = new QtListWidget;
  qtIconView->setContextMenuPolicy(Qt::CustomContextMenu);
  qtIconView->setMovement(QListView::Static);
  qtIconView->setResizeMode(QListView::Adjust);
  qtIconView->setSelectionRectVisible(true);
  qtIconView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
  qtIconView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

  connect(qtIconView, SIGNAL(itemActivated(QListWidgetItem*)), SLOT(onActivate()));
  connect(qtIconView, SIGNAL(itemSelectionChanged()), SLOT(onChange()));
  connect(qtIconView, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(onContext()));

  setFlow(iconView.state.flow);
  setOrientation(iconView.state.orientation);
  setSingleSelection(iconView.state.singleSelection);
}

void pIconView::destructor() {
  delete qtIconView;
  qtWidget = qtIconView = nullptr;
}

void pIconView::orphan() {
  destructor();
  constructor();
}

void pIconView::onActivate() {
  if(!locked() && iconView.onActivate) iconView.onActivate();
}

void pIconView::onChange() {
  for(auto& selected : iconView.state.selected) selected = false;
  for(unsigned n = 0; n < qtIconView->count(); n++) {
    if(auto item = qtIconView->item(n)) {
      if(item->isSelected()) iconView.state.selected[n] = true;
    }
  }
  if(!locked() && iconView.onChange) iconView.onChange();
}

void pIconView::onContext() {
  if(!locked() && iconView.onContext) iconView.onContext();
}

void pIconView::QtListWidget::resizeEvent(QResizeEvent* event) {
  //Qt::ScrollBarAsNeeded results in the scroll bar area being reserved from the icon viewport even when scroll bar is hidden
  //this creates the appearance of an invisible gap that wastes precious screen space
  //below code simulates a Qt::ScrollBarAsNeeded which uses the extra space when the scroll bar is hidden
  setHorizontalScrollBarPolicy(horizontalScrollBar()->maximum() > horizontalScrollBar()->minimum() ? Qt::ScrollBarAlwaysOn : Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(verticalScrollBar()->maximum() > verticalScrollBar()->minimum() ? Qt::ScrollBarAlwaysOn : Qt::ScrollBarAlwaysOff);
  return QListWidget::resizeEvent(event);
}

}

#endif
