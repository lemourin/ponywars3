#ifndef ITEMSET_HPP
#define ITEMSET_HPP
#include "SceneGraph/Item.hpp"

class World;

class ItemSet: public SceneGraph::Item {
    private:
        World* m_world;

    public:
        ItemSet(World* world);
        ~ItemSet();

        Q_INVOKABLE void dump(QString fileName);
        Q_INVOKABLE void load(QString fileName);

        void write(QJsonObject&) const;
        void read(const QJsonObject&);
};

#endif // ITEMSET_HPP
