#ifndef BOX2DCHAIN_HPP
#define BOX2DCHAIN_HPP
#include "Box2DPolygon.hpp"

class Box2DChain: public Box2DPolygon {
    private:

    protected:
        b2Shape* createShape() const;
        QSGNode* createNode() const;

    public:
        explicit Box2DChain(QQuickItem* = nullptr);

};

#endif // BOX2DCHAIN_HPP
