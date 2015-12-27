#include <QGuiApplication>
#include "Utility/Window.hpp"

int main(int argc, char** argv) {
  QGuiApplication app(argc, argv);
  // app.setOverrideCursor(Qt::BlankCursor);
  qputenv("QSG_RENDER_LOOP", "threaded");

  Window::registerTypes();

  Window window;
  QSurfaceFormat format = window.format();
  window.setFormat(format);
  window.show();

  return app.exec();
}
