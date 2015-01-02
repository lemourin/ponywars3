#include "Player.hpp"
#include "World.hpp"
#include "Weapon.hpp"
#include "Gun.hpp"
#include "Enemy.hpp"
#include "Geometry/Primitives.hpp"
#include "Geometry/Functions.hpp"
#include "Utility/DisplayItem.hpp"
#include "Lighting/LightSystem.hpp"
#include <QJsonObject>

Player::Player(Item* parent):
    Pony(parent)
    //m_hand(this),
    //m_light(this)
{
}

void Player::mouseMoved(QPointF pos) {
    //hand()->setHandPosition(mapFromItem(world(), pos));
}

void Player::mousePressed(QPointF) {
}

void Player::mouseReleased(QPointF) {
    //if (hand()->grabbedWeapon())
    //    return hand()->grabbedWeapon()->shoot();

    /*QPointF pos = mapToItem(world(), hand()->position());
    QBody* body = world()->bodyUnderPoint(pos, [](QBody* body) {
        return qobject_cast<Weapon*>(body) != nullptr;
    });

    if (body)
        hand()->grabWeapon(static_cast<Weapon*>(body));*/
}

void Player::keyPressEvent(QKeyEvent* event) {
    qDebug() << "pff";
    if (event->isAutoRepeat()) {
        event->ignore();
        return;
    }

    if (event->key() == Qt::Key_A)
        enableGoLeft();
    else if (event->key() == Qt::Key_D)
        enableGoRight();
    else if (event->key() == Qt::Key_Space) {
        if (currentState() & OnGround)
            jumpRequested();
    }
    else if (event->key() == Qt::Key_Shift) {
        if (currentState() & OnGround)
            punchRequested();
    }
    else
        event->ignore();
}

void Player::keyReleaseEvent(QKeyEvent* event) {
    /*if (event->isAutoRepeat()) {
        event->ignore();
        return;
    }

    if (event->key() == Qt::Key_A)
        disableGoLeft();
    else if (event->key() == Qt::Key_D)
        disableGoRight();
    else if (event->key() == Qt::Key_F)
        hand()->dropWeapon();
    else
        event->ignore(); */
}

void Player::beginContact(QFixture* other, b2Contact*) {
    //if (qobject_cast<Enemy*>(other->body())) {
    //    setHealth(health()-10);
    //}
}


void Player::initialize(QWorld* w) {
    Pony::initialize(w);

    for (QFixture* f = firstFixture(); f; f = f->next())
        f->setGroupIndex(-1);
    //hand()->setRange(10);

    //m_light.setLightSystem(world()->lightSystem());
    //m_light.initialize(world());
}

World* Player::world() const {
    return static_cast<World*>(Creature::world());
}

void Player::onHealthChanged() {
    if (health() <= 0) {
        //emit world()->gameOver();

        enqueueFunction([this]() {
            world()->world()->ClearForces();
            setHealth(20);
            setTransform({ 450, 600 }, 0);
            //hand()->dropWeapon();
        });
    }
}

void Player::focusOutEvent(QFocusEvent*) {
    disableState(GoingLeft | GoingRight);
}

void Player::geometryChanged(const QRectF& newGeometry,
                             const QRectF& oldGeometry) {
    Pony::geometryChanged(newGeometry, oldGeometry);

    if (newGeometry.topLeft() != oldGeometry.topLeft()) {
        //m_light.setPosition(position());
        //m_hand.updateGrabbedWeapon();
    }
}

bool Player::read(const QJsonObject& obj) {
    Pony::read(obj);

   // m_light.read(obj["light"].toObject());

    return true;
}

bool Player::write(QJsonObject& obj) const {
    Pony::write(obj);

    QJsonObject light;
    //m_light.write(light);
    obj["light"] = light;

    return true;
}



