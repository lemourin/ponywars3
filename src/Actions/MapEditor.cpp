#include "MapEditor.hpp"
#include "MainAction.hpp"

MapEditor::MapEditor(MainAction* item):
    SubAction(item),
    m_addChain(this),
    m_addBody(this),
    m_grabItem(this),
    m_deleteItem(this),
    m_bodyEdit(this),
    m_action({ addChain(), addBody(), grabItem(), deleteItem(), bodyEdit() }),
    m_object(this) {

    setWorld(item->world());
    for (Action* action: m_action)
        action->setWorld(world());
}
