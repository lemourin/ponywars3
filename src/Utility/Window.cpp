#include "Window.hpp"

#include <QDebug>
#include <QQmlContext>
#include <QQmlEngine>

Environment::Environment(QQuickView* view) : QObject(view), m_view(view) {}

Environment::System Environment::system() const {
#if defined Q_OS_ANDROID
  return System::Android;
#elif defined Q_OS_UNIX
  return System::Unix;
#elif defined Q_OS_WIN32
  return System::Win32;
#else
  return System::Unknown;
#endif
}

bool Environment::fullscreen() const {
  return view()->visibility() == QWindow::FullScreen;
}

void Environment::setFullscreen(bool enable) {
  if (fullscreen() == enable) return;

  if (enable)
    view()->showFullScreen();
  else
    view()->show();

  emit fullscreenChanged();
}

Window::Window(QWindow* parent)
    : SceneGraph::Window(parent), m_game(rootItem()), m_environment(this) {
  qmlRegisterUncreatableType<Environment>("Environment", 1, 0, "Environment",
                                          "Uncreatable type!");

  rootContext()->setContextProperty("app", &m_environment);

  World* world = m_game.view()->world();
  rootContext()->setContextProperty("world", world->object());

  world->mainAction()->registerUserInterface(rootContext());

  setSource(QUrl("qrc:/UserInterface/main.qml"));
  setResizeMode(SizeRootObjectToView);

  connect(engine(), &QQmlEngine::quit, this, &QQuickView::close);

  world->setGravity(QPointF(0, 10));
}

void Window::resizeEvent(QResizeEvent* event) {
  SceneGraph::Window::resizeEvent(event);

  QMatrix4x4 matrix;
  matrix.ortho(0, width(), height(), 0, -1, 1);

  setProjection(matrix);

  m_game.setSize(size());
}
