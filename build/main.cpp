#include <QGuiApplication>
#include "Utility/Window.hpp"

int main(int argc, char** argv) {
  QGuiApplication app(argc, argv);
  // app.setOverrideCursor(Qt::BlankCursor);

  Window::registerTypes();

  Window window;
  window.show();
  return app.exec();
}
