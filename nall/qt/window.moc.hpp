#ifndef NALL_QT_WINDOW_HPP
#define NALL_QT_WINDOW_HPP

#include <nall/base64.hpp>
#include <nall/string.hpp>

namespace nall {

class Window : public QWidget {
  Q_OBJECT

public:
  void setGeometryString(string *geometryString);
  void setCloseOnEscape(bool);
  void show();
  void hide();
  void shrink();

  Window();

protected slots:

protected:
  string *geometryString;
  bool closeOnEscape;
  void keyReleaseEvent(QKeyEvent *event);
  void closeEvent(QCloseEvent *event);
};

inline void Window::setGeometryString(string *geometryString_) {
  geometryString = geometryString_;
  if(geometryString && isVisible() == false) {
    uint8_t *data;
    unsigned length;
    base64::decode(data, length, *geometryString);
    QByteArray array((const char*)data, length);
    delete[] data;
    restoreGeometry(array);
  }
}

inline void Window::setCloseOnEscape(bool value) {
  closeOnEscape = value;
}

inline void Window::show() {
  if(geometryString && isVisible() == false) {
    uint8_t *data;
    unsigned length;
    base64::decode(data, length, *geometryString);
    QByteArray array((const char*)data, length);
    delete[] data;
    restoreGeometry(array);
  }
  QWidget::show();
  QApplication::processEvents();
  activateWindow();
  raise();
}

inline void Window::hide() {
  if(geometryString && isVisible() == true) {
    char *data;
    QByteArray geometry = saveGeometry();
    base64::encode(data, (const uint8_t*)geometry.data(), geometry.length());
    *geometryString = data;
    delete[] data;
  }
  QWidget::hide();
}

inline void Window::shrink() {
  if(isFullScreen()) return;

  for(unsigned i = 0; i < 2; i++) {
    resize(0, 0);
    usleep(2000);
    QApplication::processEvents();
  }
}

inline void Window::keyReleaseEvent(QKeyEvent *event) {
  if(closeOnEscape && (event->key() == Qt::Key_Escape)) close();
  QWidget::keyReleaseEvent(event);
}

inline void Window::closeEvent(QCloseEvent *event) {
  if(geometryString) {
    char *data;
    QByteArray geometry = saveGeometry();
    base64::encode(data, (const uint8_t*)geometry.data(), geometry.length());
    *geometryString = data;
    delete[] data;
  }
  QWidget::closeEvent(event);
}

inline Window::Window() {
  geometryString = 0;
  closeOnEscape = true;
}

}

#endif
