#ifndef ADDCHAIN_HPP
#define ADDCHAIN_HPP
#include <QQuickItem>
#include <QSGTransformNode>
#include "SubAction.hpp"

class AddChain: public SubAction {
    private:
        class Node: public QSGGeometryNode {
            public:
                Node();

                QSGGeometry* createGeometry(const std::vector<QPointF>& pts);
        };

        enum DirtyState {
            MousePos = 1 << 0,
            Points = 1 << 1,
            Finished = 1 << 2
        };

        typedef int State;

        std::vector<QPointF> m_pts;
        QPointF m_mousePos;
        State m_state;

    protected:
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void hoverMoveEvent(QHoverEvent *event);
        void keyPressEvent(QKeyEvent *event);
        //QSGNode* updatePaintNode(QSGNode *, UpdatePaintNodeData *);

    public:
        AddChain(MapEditor* = nullptr);

        inline const std::vector<QPointF>& pts() const { return m_pts; }

        void reset();

        inline QString name() const { return "AddChain"; }
};

#endif // ADDCHAIN_HPP
