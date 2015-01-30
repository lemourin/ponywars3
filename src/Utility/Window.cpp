#include "Window.hpp"
#include <QQmlEngine>
#include <QQmlContext>

#include "Entities/Player.hpp"
#include "Entities/Pony.hpp"
#include "Entities/Gun.hpp"
#include "Entities/Deagle.hpp"
#include "Entities/Enemy.hpp"

#include "QBox2D/QBody.hpp"
#include "QBox2D/Fixture/Box2DBox.hpp"
#include "QBox2D/Fixture/Box2DChain.hpp"
#include "QBox2D/Fixture/Box2DCircle.hpp"
#include "QBox2D/Fixture/Box2DEdge.hpp"
#include "QBox2D/Fixture/Box2DPolygon.hpp"

#include "Network/Server.hpp"
#include "Network/Client.hpp"

#include "Utility/Factory.hpp"

Environment::Environment(QQuickView* view):
    QObject(view),
    m_view(view) {
}

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
    if (fullscreen() == enable)
        return;

    if (enable)
        view()->showFullScreen();
    else
        view()->show();

    emit fullscreenChanged();
}

Window::Window(QWindow* parent):
    SceneGraph::Window(parent),
    m_game(rootItem()),
    m_environment(this) {

    rootContext()->setContextProperty("app", &m_environment);

    World* world = m_game.view()->world();
    rootContext()->setContextProperty("world", world->worldObject());
    rootContext()->setContextProperty("mapEditor", world->mapEditor()->mapEditorObject());

    setSource(QUrl("qrc:/qml/main.qml"));
    setResizeMode(SizeRootObjectToView);

    connect(engine(), &QQmlEngine::quit, this, &QQuickView::close);
}

void Window::resizeEvent(QResizeEvent* event) {
    SceneGraph::Window::resizeEvent(event);

    QMatrix4x4 matrix;
    matrix.ortho(0, width(), height(), 0, -1, 1);

    setProjection(matrix);

    m_game.setSize(size());
}

void Window::registerTypes() {
    qmlRegisterUncreatableType<Environment>("Environment",
                                            1, 0,
                                            "Environment",
                                            "Uncreatable type!");
    Utility::registerType<Box2DBox>("Box2DBox");
    Utility::registerType<Box2DChain>("Box2DChain");
    Utility::registerType<Box2DCircle>("Box2DCircle");
    Utility::registerType<Box2DEdge>("Box2DEdge");
    Utility::registerType<Box2DPolygon>("Box2DPolygon");

    Utility::registerType<QBody>("QBody");
    Utility::registerType<Gun>("Gun");
    Utility::registerType<Deagle>("Deagle");
    Utility::registerType<Enemy>("Enemy");
    Utility::registerType<Player>("Player");
}
