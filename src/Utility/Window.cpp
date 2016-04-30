#include "Window.hpp"

#include <QDebug>
#include <QQmlContext>
#include <QQmlEngine>

Environment::Environment(Window* view) : QObject(view), m_view(view) {}

QString Environment::system() const {
  return SceneGraph::Window::systemToString(view()->system()).c_str();
}

bool Environment::fullscreen() const {
  return view()->fullscreen();
}

void Environment::setFullscreen(bool enable) { view()->setFullScreen(enable); }

Window::Window(QWindow* parent)
    : SceneGraph::Window(parent), m_game(rootItem()), m_environment(this) {
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
