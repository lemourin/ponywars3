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
    m_environment(this) {

    rootContext()->setContextProperty("app", &m_environment);
    //rootContext()->setContextProperty("game", &m_game);
    //rootContext()->setContextProperty("world", m_game.view()->world());
    setSource(QUrl("qrc:/qml/main.qml"));

    setClearBeforeRendering(false);
    setPersistentOpenGLContext(false);
    setPersistentSceneGraph(false);
    setResizeMode(SizeRootObjectToView);

    connect(engine(), &QQmlEngine::quit,
            this, &QQuickView::close);
    connect(this, &Window::sceneGraphInvalidated,
            this, &Window::onSceneGraphInvalidated, Qt::DirectConnection);

    m_game.setParent(rootItem());
    //m_game.setZ(-1);
}

Window::~Window() {
    releaseResources();
}

void Window::onSceneGraphInvalidated() {
    for (const auto& p: m_texture)
        delete p.second;
    m_texture.clear();
}

void Window::resizeEvent(QResizeEvent* event) {
    QQuickView::resizeEvent(event);

    //m_game.setSize(size());
}

QSGTexture* Window::texture(const std::string& path) {
    QSGTexture* t = m_texture[path];
    if (t == nullptr) {
        QImage image(path.c_str());
        assert(image.isNull() == false);

        t = createTextureFromImage(image);
        t->setFiltering(QSGTexture::Linear);
        t->setMipmapFiltering(QSGTexture::Linear);

        m_texture[path] = t;
    }

    return t;
}

void Window::registerTypes() {
    qmlRegisterUncreatableType<Environment>("Environment",
                                            1, 0,
                                            "Environment",
                                            "Uncreatable type!");
    qmlRegisterType<MapEditor>();
    qmlRegisterType<BodyEdit>();
    qmlRegisterType<AddBody>();
    qmlRegisterType<AddPolygon>();
    qmlRegisterType<AddChain>();
    qmlRegisterType<AddCircle>();
    qmlRegisterType<AddRectangle>();
    qmlRegisterType<DeleteItem>();
    qmlRegisterType<GrabItem>();

    qmlRegisterType<World>();
    qmlRegisterType<DisplayItem>();

    qmlRegisterType<Player>();
    qmlRegisterType<Hand>();
    qmlRegisterType<QBody>();
    qmlRegisterType<Weapon>();

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
