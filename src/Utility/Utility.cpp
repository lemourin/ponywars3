#include "Utility.hpp"
#include <QQuickItem>
#include <QSGGeometry>
#include <QJsonObject>

void Utility::clearFocus(QQuickItem *obj) {
    obj->setFocus(false);
    for (QQuickItem* item: obj->childItems())
        clearFocus(item);
}

QJsonObject Utility::Json::toObject(QRectF rect) {
    QJsonObject obj;
    obj["x"] = rect.x();
    obj["y"] = rect.y();
    obj["w"] = rect.width();
    obj["h"] = rect.height();

    return obj;
}

QRectF Utility::Json::toRect(const QJsonObject& obj) {
    QRectF rect;
    rect.setX(obj["x"].toDouble());
    rect.setY(obj["y"].toDouble());
    rect.setWidth(obj["w"].toDouble());
    rect.setHeight(obj["h"].toDouble());

    return rect;
}

QJsonObject Utility::Json::toObject(QPointF point) {
    QJsonObject obj;
    obj["x"] = point.x();
    obj["y"] = point.y();

    return obj;
}

QPointF Utility::Json::toPoint(const QJsonObject& obj) {
    return QPointF(obj["x"].toDouble(), obj["y"].toDouble());
}
