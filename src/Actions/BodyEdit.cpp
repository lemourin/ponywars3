#include "BodyEdit.hpp"
#include "Entities/World.hpp"
#include "QBox2D/QBody.hpp"
#include "Utility/Utility.hpp"
#include <QQuickWindow>
#include <sstream>

BodyEdit::BodyEdit(MapEditor* p):
    MapEditorAction(p),
    m_body(),
    m_density(),
    m_friction(),
    m_restitution(),
    m_gravityScale(),
    m_linearDamping(),
    m_angularDamping(),
    m_dynamic() {
    //setFlag(ItemIsFocusScope);
    //setAcceptedMouseButtons(Qt::LeftButton);
}

void BodyEdit::mousePressEvent(QMouseEvent*) {
}

void BodyEdit::mouseReleaseEvent(QMouseEvent* event) {
    QBody* body = world()->bodyUnderPoint(event->localPos());

    if (body) {
        b2Fixture* fixture = body->body()->GetFixtureList();
        setDensity(fixture->GetDensity());
        setFriction(fixture->GetFriction());
        setRestitution(fixture->GetRestitution());
        setGravityScale(body->gravityScale());
        setLinearDamping(body->linearDamping());
        setAngularDamping(body->angularDamping());
        setDynamic(body->bodyType() == QBody::Dynamic);
    }

    setBody(body);

}

void BodyEdit::reset() {
    Action::reset();

    //window()->contentItem()->setFocus(true);
    setBody(nullptr);
}

void BodyEdit::setBody(QBody* body) {
    if (m_body == body)
        return;
    m_body = body;
    //emit bodyChanged();
}

void BodyEdit::setDensity(qreal d) {
    if (qIsNull(m_density-d))
        return;
    m_density = d;
    //emit densityChanged();
}

void BodyEdit::setFriction(qreal f) {
    if (qIsNull(m_friction-f))
        return;
    m_friction = f;
    //emit frictionChanged();
}

void BodyEdit::setRestitution(qreal r) {
    if (qIsNull(m_restitution-r))
        return;
    m_restitution = r;
    //emit restitutionChanged();
}

void BodyEdit::setGravityScale(qreal g) {
    if (qIsNull(m_gravityScale-g))
        return;
    m_gravityScale = g;
    //emit gravityScaleChanged();
}

void BodyEdit::setLinearDamping(qreal l) {
    if (qIsNull(m_linearDamping-l))
        return;
    m_linearDamping = l;
    //emit linearDampingChanged();
}

void BodyEdit::setAngularDamping(qreal a) {
    if (qIsNull(m_angularDamping-a))
        return;
    m_angularDamping = a;
    //emit angularDampingChanged();
}

void BodyEdit::setDynamic(bool d) {
    if (m_dynamic == d)
        return;
    m_dynamic = d;
    //emit dynamicChanged();
}

void BodyEdit::applyChanges() const {
    if (!body())
        return;

    for (b2Fixture* f = m_body->body()->GetFixtureList(); f; f = f->GetNext()) {
        f->SetDensity(density());
        f->SetFriction(friction());
        f->SetRestitution(restitution());
    }

    body()->setGravityScale(gravityScale());
    body()->setLinearDamping(linearDamping());
    body()->setAngularDamping(angularDamping());

    if (dynamic())
        body()->setBodyType(QBody::Dynamic);
    else
        body()->setBodyType(QBody::Static);
}

