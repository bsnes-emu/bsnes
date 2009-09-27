class QbDiskBrowser;

class QbDiskBrowserView : public QTreeView {
  Q_OBJECT

public:
  QbDiskBrowser &parent;
  void keyPressEvent(QKeyEvent*);
  void keyReleaseEvent(QKeyEvent*);
  QbDiskBrowserView(QbDiskBrowser&);

signals:
  void cdUp();
  void changed(const QModelIndex&);

public slots:
  void currentChanged(const QModelIndex&, const QModelIndex&);
};

class QbDiskBrowser : public QbWindow {
  Q_OBJECT

public:
  QGroupBox *panel();
  QPushButton *acceptButton();

  void setPath(const QString&);
  void setNameFilters(const QString&);

  QbDiskBrowser();

signals:
  void itemActivated(const QString&, bool);
  void itemChanged(const QString&, bool);

public slots:
  void cdUp();
  void updatePath();
  void updateFilter();
  void activateItem(const QModelIndex&);
  void changeItem(const QModelIndex&);

protected:
  QVBoxLayout *layout;
  QHBoxLayout *topLayout;
  QVBoxLayout *browseLayout;
    QComboBox *path;
    QbDiskBrowserView *view;
  QVBoxLayout *panelLayout;
    QGroupBox *group;
  QHBoxLayout *controlLayout;
    QComboBox *filter;
    QPushButton *options;
    QPushButton *load;
    QPushButton *cancel;

  QFileSystemModel *model;

  friend class QbDiskBrowserListView;
};
