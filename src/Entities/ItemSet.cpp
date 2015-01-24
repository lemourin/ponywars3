#include "ItemSet.hpp"
#include "World.hpp"
#include "Utility/Factory.hpp"
#include <fstream>
#include <iostream>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

ItemSet::ItemSet(World* world):
    SceneGraph::Item(world),
    m_world(world) {
}

ItemSet::~ItemSet() {
    while (firstChild())
        delete firstChild();
}

void ItemSet::dump(QString /*fileName*/) {
    //std::fstream file(fileName.toStdString(), std::fstream::out);
    //std::ostream& file = std::cout;

    QJsonObject object;
    write(object);

    QJsonDocument document(object);
    qDebug() << document.toJson();

}

void ItemSet::load(QString fileName) {
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        qDebug() << "failed to open map file";

    QByteArray data = file.readAll();

    QJsonDocument document(QJsonDocument::fromJson(data));
    read(document.object());
}

void ItemSet::write(QJsonObject& obj) const {
    QJsonArray array;
    /*for (QQuickItem* item: childItems()) {
        if (QBody* body = qobject_cast<QBody*>(item)) {
            QJsonObject object;
            if (body->write(object))
                array.append(object);
        }
    }*/

    obj["bodies"] = array;
}

void ItemSet::read(const QJsonObject& obj) {
    QJsonArray array = obj["bodies"].toArray();

    for (int i=0; i<array.size(); i++) {
        QJsonObject obj = array[i].toObject();
        QByteArray className = obj["class"].toString().toLocal8Bit();
        QBody* body = Utility::create<QBody>(className);
        assert(body != nullptr);

        body->setParent(this);
        body->read(obj);
        body->initialize(m_world);
    }
}

