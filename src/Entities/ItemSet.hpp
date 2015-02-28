#ifndef ITEMSET_HPP
#define ITEMSET_HPP
#include "SceneGraph/Item.hpp"

class World;
class QBody;

class ItemSet: public SceneGraph::Item {
    private:
        World* m_world;
        std::vector<QBody*> m_body;

    public:
        ItemSet(World* world);
        ~ItemSet();

        void addBody(QBody*);
        void removeBody(QBody*);

        void dump(QString fileName);
        void load(QString fileName);

        void write(QJsonObject&) const;
        void read(const QJsonObject&);
};

#endif // ITEMSET_HPP
