#ifndef GRABITEM_HPP
#define GRABITEM_HPP
#include <QPointer>
#include <QSGGeometryNode>
#include <QSGGeometry>
#include "MapEditorAction.hpp"
#include "QBox2D/QJoint.hpp"

class QWorld;
class QBody;
class b2MouseJoint;

class ArrowNode: public QSGGeometryNode {
    private:
        QSGGeometry m_geometry;
        QPointF m_p1, m_p2;

    public:
        ArrowNode();

        inline void setP1(const QPointF& p) { m_p1 = p; }
        inline void setP2(const QPointF& p) { m_p2 = p; }

        void updateGeometry();
};

class GrabItem: public MapEditorAction {
    private:
        Q_OBJECT

        QMouseJoint* m_mouseJoint;
        QBody* m_grabbedBody;

        void destroyJoint();

    protected:
        //QSGNode* updatePaintNode(QSGNode *, UpdatePaintNodeData *);
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);

    public:
        GrabItem(MapEditor*);
        ~GrabItem();

        bool pickItem(QPointF point);
        void releaseItem();
        void setDirection(QPointF);

        void reset();
};

#endif // GRABITEM_HPP
