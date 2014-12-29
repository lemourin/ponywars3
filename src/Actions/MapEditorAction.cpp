#include "MapEditorAction.hpp"
#include "MapEditor.hpp"

MapEditorAction::MapEditorAction(MapEditor* parent):
    Action(parent),
    m_mapEditor(parent) {
}

void MapEditorAction::focusChanged() {
    Action::focusChanged();
    m_mapEditor->actionFocusChanged(this);
}
