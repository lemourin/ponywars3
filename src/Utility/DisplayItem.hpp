#ifndef DISPLAYITEM_HPP
#define DISPLAYITEM_HPP

#include "SceneGraph/Item.hpp"

class BaseItem;
class DisplayItem;
class QWheelEvent;
class QHoverEvent;

class DisplayItemFrame: public SceneGraph::Item {
    private:
        friend class DisplayItem;

        DisplayItem* m_displayItem;

    protected:
        void geometryChanged(const QRectF &newGeometry,
                             const QRectF &oldGeometry);

    public:
        explicit DisplayItemFrame(SceneGraph::Item* = nullptr);
        ~DisplayItemFrame();

};

class DisplayItem: public SceneGraph::Item {
    private:
        friend class DisplayItemFrame;
        friend class BaseItem;

        DisplayItemFrame* m_frame;
        BaseItem* m_focusedObject;

        QRectF m_visibleArea;
        QPointF m_effectiveLookAt;
        QPointF m_lookAt;
        bool m_flickable;

        QPointF m_startPoint;
        qreal m_distance;

        qreal m_scale;

        void focusedObjectPositionChanged();

        void returnLookAtToBounds();
        void returnScaleToBounds();
        void updateVisibleArea();

    protected:
        void wheelEvent(QWheelEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void hoverMoveEvent(QHoverEvent *event);
        void touchEvent(QTouchEvent *event);
        void geometryChanged(const QRectF &newGeometry,
                             const QRectF &oldGeometry);

        virtual void visibleAreaChanged();

        virtual void mouseMoved(QPointF);
        virtual void mousePressed(QPointF);
        virtual void mouseReleased(QPointF);

    public:
        explicit DisplayItem(DisplayItemFrame *parent = nullptr);
        ~DisplayItem();

        inline qreal scale() const { return m_scale; }
        void setScale(qreal);

        inline QRectF visibleArea() const { return m_visibleArea; }
        void setVisibleArea(QRectF);

        inline QPointF lookAt() const { return m_lookAt; }
        void setLookAt(QPointF);

        inline QPointF effectiveLookAt() const { return m_effectiveLookAt; }
        void setEffectiveLookAt(QPointF);

        inline BaseItem* focusedObject() const { return m_focusedObject; }
        void setFocusedObject(BaseItem*);

        inline bool flickable() const { return m_flickable; }
        void setFlickable(bool);

};

#endif // DISPLAYITEM_HPP
