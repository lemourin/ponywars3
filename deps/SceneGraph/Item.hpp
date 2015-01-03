#ifndef ITEM_HPP
#define ITEM_HPP
#include "BaseObject.hpp"
#include <QMatrix4x4>

class QKeyEvent;
class QTouchEvent;
class QMouseEvent;

namespace SceneGraph {

class Window;
class Node;
class TransformNode;

class Item: protected BaseObject {
    private:
        friend class Window;
        friend class Renderer;

        Window* m_window;
        TransformNode* m_itemNode;
        Node* m_node;
        unsigned m_state;
        QMatrix4x4 m_matrix;

        enum State {
            ScheduledUpdate = 1 << 0,
            ParentChanged = 1 << 1,
            ModelMatrixChanged = 1 << 2,
            HasFocus = 1 << 3,
            Visible = 1 << 4,
            VisibleChanged = 1 << 5
        };

        void setWindow(Window*);

    protected:
        virtual Node* synchronize(Node* old);

        virtual void visibleChanged();
        virtual void focusChanged();
        virtual void matrixChanged();

        virtual void keyPressEvent(QKeyEvent*);
        virtual void keyReleaseEvent(QKeyEvent*);
        virtual void touchEvent(QTouchEvent*);
        virtual void mouseMoveEvent(QMouseEvent*);
        virtual void timerEvent(QTimerEvent*);

    public:
        Item(Item* parent = nullptr);
        ~Item();

        Item* firstChild() const;
        Item* next() const;

        void appendChild(Item*);
        void removeChild(Item*);

        inline Window* window() const { return m_window; }

        Item* parent() const;
        void setParent(Item*);

        inline const QMatrix4x4& matrix() const { return m_matrix; }
        void setMatrix(const QMatrix4x4& m);

        void resetTransform();
        void translate(qreal x, qreal y);
        void scale(qreal x, qreal y);
        void rotate(qreal angle, qreal x, qreal y, qreal z);

        QMatrix4x4 effectiveMatrix() const;

        QPointF mapToItem(Item*, QPointF);
        QPointF mapToScreen(QPointF);

        QPointF mapFromItem(Item*, QPointF);
        QPointF mapFromScreen(QPointF);

        inline bool focus() const { return m_state & HasFocus; }
        void setFocus(bool);

        inline bool visible() const { return m_state & Visible; }
        void setVisible(bool);

        int startTimer(int interval);
        void killTimer(int timerId);

        void update();
};

}

#endif // ITEM_HPP
