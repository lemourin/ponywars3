#include <QGuiApplication>
#include "Utility/Window.hpp"

int main(int argc, char** argv) {
  QGuiApplication app(argc, argv);
  // app.setOverrideCursor(Qt::BlankCursor);

  qputenv("QML_FORCE_THREADED_RENDERER", "1");

  Window::registerTypes();

  Window window;
  window.show();

  return app.exec();
}
