#include <QGuiApplication>
#include "Utility/Window.hpp"

int main(int argc, char** argv) {
    QGuiApplication app(argc, argv);
    //app.setOverrideCursor(Qt::BlankCursor);

    qputenv("QML_FORCE_THREADED_RENDERER", "1");

    Utility::Engine::instance()->init();
    Window::registerTypes();

    Window window;
    window.show();

    int exitCode = app.exec();

    Utility::Engine::instance()->destroy();

    return exitCode;
}
