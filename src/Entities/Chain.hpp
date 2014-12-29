#ifndef CHAIN_HPP
#define CHAIN_HPP
#include "QBox2D/QBody.hpp"
#include "Geometry/Circle.hpp"
#include <QSGGeometryNode>

class Chain: public QBody {
    private:
        std::vector<QPointF> m_vertices;

        void createChain();
        void beginContact(QFixture* other, b2Contact*);

    protected:
        //QSGNode* updatePaintNode(QSGNode *, UpdatePaintNodeData *);

    public:
        Chain(SceneGraph::Item* = nullptr);
        ~Chain();

        const std::vector<QPointF>& vertices() const { return m_vertices; }
        void setVertices(const std::vector<QPointF>& v);

        void cutCircle(Circle);

        bool testPoint(const QPointF& point) const;


};

#endif // CHAIN_HPP
