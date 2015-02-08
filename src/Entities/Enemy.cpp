#include "Enemy.hpp"
#include "Gun.hpp"
#include "World.hpp"
#include "Player.hpp"
#include <QJsonObject>

Enemy::Enemy(SceneGraph::Item* parent):
    Pony(parent),
    m_target() {
}

void Enemy::healthChanged() {
    //if (health() <= 0)
    //    deleteLater();
}

void Enemy::timerEvent(QTimerEvent*) {
    setTarget(static_cast<World*>(world())->player());

    if (!target())
        return;
    if (canSee(target())) {
        if (target()->position().x() < position().x()) {
            disableGoRight();
            enableGoLeft();
        }
        else {
            disableGoLeft();
            enableGoRight();
        }

        /*if (target()->y() < y())
            emit requestJump();*/
    }
    else {
        disableGoLeft();
        disableGoRight();
        //emit requestJump();
    }
}

void Enemy::beginContact(QFixture*, b2Contact*) {
    //if (qobject_cast<Bullet*>(other->body())) {
    //    setHealth(health()-5);
    //}
}

