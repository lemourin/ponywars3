#include "QWorld.hpp"

#include "QBody.hpp"
#include "QFixture.hpp"
#include "QJoint.hpp"

#include <QTimerEvent>
#include <QQuickWindow>

#include <Box2D/Box2D.h>

void QContactListener::BeginContact(b2Contact *contact) {
    QFixture* fixtureA = QFixture::toQFixture(contact->GetFixtureA());
    QFixture* fixtureB = QFixture::toQFixture(contact->GetFixtureB());

    if (!fixtureA || !fixtureB)
        return;

    fixtureA->body()->beginContact(fixtureB, contact);
    fixtureB->body()->beginContact(fixtureA, contact);
}

void QContactListener::EndContact(b2Contact *contact) {
    QFixture* fixtureA = QFixture::toQFixture(contact->GetFixtureA());
    QFixture* fixtureB = QFixture::toQFixture(contact->GetFixtureB());

    if (!fixtureA || !fixtureB)
        return;

    fixtureA->body()->endContact(fixtureB, contact);
    fixtureB->body()->endContact(fixtureA, contact);
}

void QContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
    QFixture* fixtureA = QFixture::toQFixture(contact->GetFixtureA());
    QFixture* fixtureB = QFixture::toQFixture(contact->GetFixtureB());

    if (!fixtureA || !fixtureB)
        return;

    fixtureA->body()->preSolve(fixtureB, contact, oldManifold);
    fixtureB->body()->preSolve(fixtureA, contact, oldManifold);
}

void QContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
    QFixture* fixtureA = QFixture::toQFixture(contact->GetFixtureA());
    QFixture* fixtureB = QFixture::toQFixture(contact->GetFixtureB());

    if (!fixtureA || !fixtureB)
        return;

    fixtureA->body()->postSolve(fixtureB, contact, impulse);
    fixtureB->body()->postSolve(fixtureA, contact, impulse);
}

void QDestructionListener::SayGoodbye(b2Joint* j) {
    QJoint* joint = QJoint::toQJoint(j);
    if (joint)
        joint->destroyJoint();
}

void QDestructionListener::SayGoodbye(b2Fixture* f) {
    QFixture* fixture = QFixture::toQFixture(f);
    if (fixture)
        fixture->destroyFixture();
}

QWorld::QWorld(SceneGraph::Item* parent):
    SceneGraph::Item(parent),
    m_running(false),
    m_locked(false),
    m_timeStep(1.0f / 60.0f),
    m_velocityIterations(10),
    m_positionIterations(10),
    m_frameTime(1000 / 60),
    m_world(b2Vec2(0, 10)),
    m_groundBody(this),
    m_timer(-1),
    m_glinitialized() {

    //setFlag(ItemIsFocusScope);

    world()->SetContactListener(&m_contactListener);
    world()->SetDestructionListener(&m_destructionListener);

    m_groundBody.initialize(this);
}

QWorld::~QWorld() {
    destroyBodies();
}

void QWorld::initialize() {
    setRunning(true);
}

void QWorld::destroyBodies() {
    while (b2Body* body = world()->GetBodyList()) {
        QBody* qbody = QBody::toQBody(body);
        assert(qbody);

        qbody->destroyBody();
    }
}

void QWorld::setRunning(bool running) {
    if (m_running == running)
        return;

    m_running = running;

    if (running) {
        m_timer = startTimer(m_frameTime);
    }
    else {
        killTimer(m_timer);
        m_timer = -1;
    }
}

QPointF QWorld::gravity() const {
    return QPointF(world()->GetGravity().x,
                   world()->GetGravity().y);
}

void QWorld::setGravity(const QPointF &gravity) {
    world()->SetGravity(b2Vec2(gravity.x(), gravity.y()));
}

std::vector<QBody*> QWorld::bodies() {
    std::vector<QBody*> result;
    for (b2Body* body = world()->GetBodyList(); body; body = body->GetNext())
        if (QBody::toQBody(body))
            result.push_back(QBody::toQBody(body));
    return result;
}

std::vector<const QBody*> QWorld::bodies() const {
    std::vector<const QBody*> result;
    for (const b2Body* body = world()->GetBodyList(); body; body = body->GetNext())
        if (QBody::toQBody(body))
            result.push_back(QBody::toQBody(body));
    return result;
}

void QWorld::step() {
    m_locked = true;

    world()->Step(timeStep(), velocityIterations(), positionIterations());

    std::vector<QBody*> current = m_enqueued;
    m_enqueued = { };
    for (QBody* body: current) {
        std::queue< std::function<void()> > work = body->m_work;
        body->m_work = std::queue< std::function<void()> >();

        while (!work.empty()) {
            work.front()();
            work.pop();
        }
    }

    std::vector<QBody*> oldBodies = visibleBodies();
    m_visibleBodies = bodies(visibleRect());

    const std::vector<QBody*>& currentBodies = visibleBodies();

    size_t it1 = 0, it2 = 0;
    while (it1 < oldBodies.size()) {
        while (it2 < currentBodies.size() && oldBodies[it1] > currentBodies[it2])
            it2++;
        if (it2 == currentBodies.size() || oldBodies[it1] != currentBodies[it2]) {
            oldBodies[it1]->setVisible(false);
            oldBodies[it1]->synchronize();
        }
        it1++;
    }

    for (QBody *body: visibleBodies()) {
        body->setVisible(true);
        body->synchronize();
    }

    m_locked = false;
}

void QWorld::timerEvent(QTimerEvent* event) {
    if (event->timerId() == m_timer)
        step();

    SceneGraph::Item::timerEvent(event);
}

/*void QWorld::itemChange(ItemChange change, const ItemChangeData& data) {
    QQuickItem::itemChange(change, data);
    if (change == ItemSceneChange && data.window) {
        connect(data.window, &QQuickWindow::beforeRendering,
                this, &QWorld::onBeforeRendering, Qt::DirectConnection);
    }
}*/

std::vector<QBody*> QWorld::bodies(const QRectF& rect) const {
    b2AABB b2rect;
    b2rect.lowerBound = b2Vec2(rect.topLeft().x(), rect.topLeft().y());
    b2rect.upperBound = b2Vec2(rect.bottomRight().x(), rect.bottomRight().y());

    BodyFinder callback;
    world()->QueryAABB(&callback, b2rect);

    auto& objs = callback.m_foundBodies;

    std::sort(objs.begin(), objs.end());
    objs.resize(std::unique(objs.begin(), objs.end())-objs.begin());

    return objs;
}

QBody* QWorld::bodyUnderPoint(const QPointF& p,
                              std::function<bool(QBody*)> ok) const {
    for (QBody* body: visibleBodies()) {
        bool hasSensor = false;
        for (QFixture* f = body->firstFixture(); f; f = f->next()) {
            hasSensor |= f->isSensor();
            if (hasSensor)
                break;
        }

        if (!hasSensor && body->testPoint(p) && (!ok || ok(body)))
            return body;
    }
    return nullptr;
}

QFixture* QWorld::fixtureUnderPoint(const QPointF&) const {
    return nullptr;
}

std::vector<QFixture*> QWorld::fixtures(const QRectF& rect) const {
    b2AABB box;
    box.lowerBound = b2Vec2(rect.x(), rect.y());
    box.upperBound = b2Vec2(rect.x()+rect.width(), rect.y()+rect.height());

    FixtureFinder callback;
    world()->QueryAABB(&callback, box);

    auto& objs = callback.m_foundFixtures;
    std::sort(objs.begin(), objs.end());
    objs.resize(std::unique(objs.begin(), objs.end())-objs.begin());

    return objs;
}

void QWorld::rayCast(b2RayCastCallback* raycast, QPointF p1, QPointF p2) const {
    world()->RayCast(raycast, b2Vec2(p1.x(), p1.y()), b2Vec2(p2.x(), p2.y()));
}

void QWorld::onBodyDestroyed(QBody* body) {
    assert(locked() == false);

    auto it = std::find(m_visibleBodies.begin(), m_visibleBodies.end(), body);
    if (it != m_visibleBodies.end())
        m_visibleBodies.erase(it);

    it = std::find(m_enqueued.begin(), m_enqueued.end(), body);
    if (it != m_enqueued.end())
        m_enqueued.erase(it);

}

void QWorld::onBeforeRendering() {
    if (!m_glinitialized) {
        initializeOpenGLFunctions();
        initializeGL();

        m_glinitialized = true;
    }

    //window()->resetOpenGLState();
    renderGL();
}

void QWorld::initializeGL() {
    qDebug() << reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << reinterpret_cast<const char*>(glGetString(GL_RENDERER));
}

void QWorld::renderGL() {
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool QWorld::BodyFinder::ReportFixture(b2Fixture* fixture) {
    m_foundBodies.push_back(QBody::toQBody(fixture->GetBody()));
    return true;
}

bool QWorld::FixtureFinder::ReportFixture(b2Fixture* fixture) {
    m_foundFixtures.push_back(QFixture::toQFixture(fixture));
    return true;
}

