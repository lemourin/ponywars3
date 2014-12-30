#include "Box2DBox.hpp"
#include "Utility/Utility.hpp"
#include <QJsonObject>
#include <QSGSimpleRectNode>

Box2DBox::Box2DBox(Item* parent):
    Fixture(parent),
    m_texturedRectangle(this),
    m_normalMapRectangle(this) {
    setTexture(&m_texturedRectangle);
    setNormalMap(&m_normalMapRectangle);
}

void Box2DBox::setTextureRect(QRectF rect) {
    m_texturedRectangle.setTextureRect(rect);
    m_normalMapRectangle.setTextureRect(rect);
}

std::vector<QPointF> Box2DBox::vertices() const {
    return {
        QPointF(0, 0),
        QPointF(size().width(), 0),
        QPointF(size().width(), size().height()),
        QPointF(0, size().height()),
        QPointF(0, 0)
    };
}

void Box2DBox::setSize(QSizeF) {
}

bool Box2DBox::read(const QJsonObject& obj) {
    Fixture::read(obj);

    setSize(QSizeF(obj["width"].toDouble(), obj["height"].toDouble()));
    setTextureRect(Utility::Json::toRect(obj["textureRect"].toObject()));
    return true;
}

bool Box2DBox::write(QJsonObject& obj) const {
    Fixture::write(obj);

    obj["width"] = size().width();
    obj["height"] = size().height();
    obj["textureRect"] = Utility::Json::toObject(textureRect());
    return true;
}

/*QSGNode* Box2DBox::createNode() const {
    QSGSimpleRectNode* node = new QSGSimpleRectNode;
    node->setRect(0, 0, width(), height());
    node->setColor(QColor(255, 0, 0));

    return node;
}*/

b2Shape* Box2DBox::createShape() const {
    b2Vec2 vertices[4];
    vertices[0].Set(position().x(), position().y());
    vertices[1].Set(position().x() + size().width(), position().y());
    vertices[2].Set(position().x() + size().width(), position().y() + size().height());
    vertices[3].Set(position().x(), position().y() + size().height());

    b2PolygonShape *shape = new b2PolygonShape;
    shape->Set(vertices, 4);

    return shape;
}
