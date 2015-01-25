#include "World.hpp"
#include <QQuickWindow>
#include <QJsonObject>
#include <QJsonDocument>
#include <fstream>
#include <unordered_set>
#include "QBox2D/QFixture.hpp"
#include "QBox2D/QBody.hpp"
#include "Lighting/LightSystem.hpp"
#include "Entities/Player.hpp"
#include "Entities/Gun.hpp"
#include "Entities/ItemSet.hpp"
#include "Entities/Game.hpp"
#include "Utility/Utility.hpp"
#include "Utility/Factory.hpp"

Boundary::Boundary(qreal width,
                   qreal height,
                   qreal thickness,
                   Item* p):
    QBody(p),
    m_left(this),
    m_right(this),
    m_top(this),
    m_bottom(this) {

    m_left.translate(0, 0);
    m_left.setSize(QSizeF(thickness, height));
    m_right.translate(width-thickness, 0);
    m_right.setSize(QSizeF(thickness, height));

    m_top.translate(0, 0);
    m_top.setSize(QSizeF(width, thickness));
    m_bottom.translate(0, height-thickness);
    m_bottom.setSize(QSizeF(width, thickness));
}

World::World(ViewWorld* viewWorld):
    QWorld(viewWorld),
    m_viewWorld(viewWorld),
    m_player(),
    m_fps(),
    m_boundary(),
    m_itemSet(this),
    m_mapEditor(this) {

    //setAcceptedMouseButtons(Qt::LeftButton);
}

World::~World() {
    delete m_player;
}

void World::updateFps() {
    qreal t = m_fpscounter.restart();

    if (!qIsNull(t))
        setFps(1000.0/t);
}

void World::mousePressEvent(QMouseEvent* event) {    
    /*if (player() && !player()->hasActiveFocus() && !view()->flickable())
        player()->forceActiveFocus();
    else
        event->ignore();*/
}

void World::geometryChanged(const QRectF& newGeometry,
                            const QRectF& oldGeometry) {
    /*QWorld::geometryChanged(newGeometry, oldGeometry);

    delete m_boundary;
    m_boundary = nullptr;

    if (!qIsNull(width()) && !qIsNull(height())) {
        m_boundary = new Boundary(width(), height(), 0.1, this);
        m_boundary->setOpacity(0);
        m_boundary->initialize();
    }

    m_mapEditor.setSize(newGeometry.size());

    lightSystem()->worldSizeChanged();*/
}

/*void World::itemChange(ItemChange change, const ItemChangeData& data) {
    QWorld::itemChange(change, data);

    if (change == ItemSceneChange && data.window) {
        m_fpscounter.restart();
        connect(data.window, &QQuickWindow::beforeRendering,
                this, &World::updateFps);
    }
}*/

void World::onBodyDestroyed(QBody* body) {
    if (player() == body)
        setPlayer(nullptr);

    QWorld::onBodyDestroyed(body);
}

void World::onBodyAdded(QBody* body) {
    QWorld::onBodyAdded(body);

    assert(lightSystem());
    lightSystem()->addBody(body);
}

void World::setPlayer(Player* player) {
    if (m_player == player)
        return;

    m_player = player;

    if (player) {
        player->setFocus(true);
        view()->setFocusedObject(player);
    }

    //emit playerChanged();
}

LightSystem* World::lightSystem() const {
    return view()->game()->lightSystem();
}

void World::setFps(qreal f) {
    if (qIsNull(m_fps-f))
        return;
    m_fps = f;
    //emit fpsChanged();
}

void World::setPaused(bool p) {
    if (paused() == p)
        return;
    setRunning(!p);
    setFocus(!p);
    //emit pausedChanged();
}

void World::read(const QJsonObject& obj) {
    m_itemSet.read(obj);

    QJsonObject p = obj["player"].toObject();
    Player* player = Utility::create<Player>(p["class"].toString().toLocal8Bit());
    assert(player);

    player->setParent(this);
    player->read(p);
    player->initialize(this);

    setPlayer(player);
}

void World::write(QJsonObject& obj) const {
    m_itemSet.write(obj);

    if (player()) {
        QJsonObject p;
        player()->write(p);
        obj["player"] = p;
    }
}

