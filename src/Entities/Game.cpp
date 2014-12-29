#include "Game.hpp"
#include <QQuickWindow>
#include <QJsonObject>
#include <QJsonDocument>

Game::Game(Item* parent):
    DisplayItemFrame(parent),
    m_viewWorld(this),
    m_lightSystem(this) {

    m_viewWorld.initialize();
    m_lightSystem.initialize();

    load(":/json/map00.json");
    setClip(true);
}

bool Game::read(const QJsonObject& obj) {
    m_viewWorld.read(obj["viewWorld"].toObject());
    m_lightSystem.read(obj["lightSystem"].toObject());

    return true;
}

bool Game::write(QJsonObject& obj) const {
    QJsonObject viewWorld;
    m_viewWorld.write(viewWorld);
    obj["viewWorld"] = viewWorld;

    QJsonObject lightSystem;
    m_lightSystem.write(lightSystem);
    obj["lightSystem"] = lightSystem;

    return true;
}

void Game::geometryChanged(const QRectF& newGeometry,
                           const QRectF& oldGeometry) {
    DisplayItemFrame::geometryChanged(newGeometry, oldGeometry);

    m_lightSystem.setSize(newGeometry.size());

#ifdef Q_OS_ANDROID
    const QSize maxSize(800, 600);
#else
    const QSize maxSize(1920, 1080);
#endif

    QSize size(std::min(int(width()), maxSize.width()),
               std::min(int(height()), maxSize.height()));
    m_lightSystem.setResolution(size);

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


