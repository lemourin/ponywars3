#include "Game.hpp"
#include <QQuickWindow>
#include <QJsonObject>
#include <QJsonDocument>

Game::Game(Item* parent):
    DisplayItemFrame(parent),
    m_viewWorld(new ViewWorld(this)),
    m_lightSystem(new LightSystem(this)) {

    view()->initialize();
    lightSystem()->initialize();

    load(":/json/map00.json");
}

Game::~Game() {
    delete m_viewWorld;
    m_viewWorld = nullptr;

    delete m_lightSystem;
    m_lightSystem = nullptr;
}

void Game::sizeChanged() {
    DisplayItemFrame::sizeChanged();

#ifdef Q_OS_ANDROID
    const QSize maxres(800, 600);
#else
    const QSize maxres(1920, 1080);
#endif

    QSize resolution(std::min(int(size().width()), maxres.width()),
                     std::min(int(size().height()), maxres.height()));

    lightSystem()->setResolution(resolution);
    lightSystem()->setSize(size());
}

bool Game::read(const QJsonObject& obj) {
    view()->read(obj["viewWorld"].toObject());
    lightSystem()->read(obj["lightSystem"].toObject());

    return true;
}

bool Game::write(QJsonObject& obj) const {
    QJsonObject viewWorld;
    view()->write(viewWorld);
    obj["viewWorld"] = viewWorld;

    QJsonObject lights;
    lightSystem()->write(lights);
    obj["lightSystem"] = lights;

    return true;
}

void Game::load(QString path) {
    QFile file(path);
    if (!file.open(QFile::ReadOnly))
        qDebug() << "failed to open map file";

    QByteArray data = file.readAll();

    QJsonDocument document(QJsonDocument::fromJson(data));
    read(document.object());

}

void Game::dump(QString /*path*/) const {
    QJsonObject obj;
    write(obj);

    QJsonDocument document(obj);

    qDebug() << document.toJson();
}


