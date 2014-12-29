#ifndef DELETEITEM_HPP
#define DELETEITEM_HPP
#include "MapEditorAction.hpp"

class DeleteItem: public MapEditorAction {
    private:
        Q_OBJECT

    protected:
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);

    public:
        DeleteItem(MapEditor*);
};

#endif // DELETEITEM_HPP
