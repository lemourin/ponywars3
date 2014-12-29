#ifndef MAPEDITORACTION_HPP
#define MAPEDITORACTION_HPP
#include "Action.hpp"

class MapEditor;

class MapEditorAction: public Action {
    private:
        MapEditor* m_mapEditor;

    protected:
        void focusChanged();

    public:
       MapEditorAction(MapEditor* parent);
};

#endif // MAPEDITORACTION_HPP
