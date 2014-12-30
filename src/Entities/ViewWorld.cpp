#include "ViewWorld.hpp"
#include "Player.hpp"
#include "Game.hpp"
#include <QQuickWindow>
#include <QJsonObject>

ViewWorld::ViewWorld(Game* game):
    DisplayItem(game),
    m_world(this),
    m_game(game) {

}

void ViewWorld::initialize() {
    m_world.initialize();

    m_world.setFocus(true);
    setFocus(true);
}

bool ViewWorld::read(const QJsonObject& obj) {
    m_world.read(obj["world"].toObject());

    //setWidth(obj["width"].toDouble());
    //setHeight(obj["height"].toDouble());
    setScale(obj["scale"].toDouble());

    return true;
}

bool ViewWorld::write(QJsonObject& obj) const {
    QJsonObject world;
    m_world.write(world);
    obj["world"] = world;

    //obj["width"] = width();
    //obj["height"] = height();
    obj["scale"] = scale();

    return true;
}

bool ViewWorld::sendEvent() const {
    return false;
    //return world() && world()->player() && world()->player()->hasActiveFocus();
}

void ViewWorld::mouseMoved(QPointF p) {
    if (sendEvent())
        world()->player()->mouseMoved(p);
}

void ViewWorld::mousePressed(QPointF p) {
    if (sendEvent())
        world()->player()->mousePressed(p);
}

void ViewWorld::mouseReleased(QPointF p) {
    if (sendEvent())
        world()->player()->mouseReleased(p);
}

void ViewWorld::geometryChanged(const QRectF& newGeometry,
                                const QRectF& oldGeometry) {
    DisplayItem::geometryChanged(newGeometry, oldGeometry);

    //m_world.setSize(newGeometry.size());
}

void ViewWorld::visibleAreaChanged() {
    DisplayItem::visibleAreaChanged();

    game()->lightSystem()->visibleAreaChanged();
    world()->setVisibleRect(visibleArea());
}

