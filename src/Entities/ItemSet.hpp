#ifndef ITEMSET_HPP
#define ITEMSET_HPP
#include "SceneGraph/Item.hpp"

class ItemSet: public SceneGraph::Item {
    private:
        Q_OBJECT

    public:
        ItemSet(SceneGraph::Item* = nullptr);

        Q_INVOKABLE void dump(QString fileName);
        Q_INVOKABLE void load(QString fileName);

        void write(QJsonObject&) const;
        void read(const QJsonObject&);
};

#endif // ITEMSET_HPP
