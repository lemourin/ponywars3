#ifndef DELETEITEM_HPP
#define DELETEITEM_HPP
#include "SubAction.hpp"

class DeleteItem: public SubAction {
    protected:
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);

    public:
        DeleteItem(MapEditor*);

        inline QString name() const { return "DeleteItem"; }
};

#endif // DELETEITEM_HPP
