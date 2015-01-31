#include "MapEditor.hpp"
#include "Entities/Game.hpp"
#include "Entities/World.hpp"
#include <cassert>

MapEditor::MapEditor(World* item):
    Action(item),
    m_addChain(this),
    m_addBody(this),
    m_grabItem(this),
    m_deleteItem(this),
    m_bodyEdit(this),
    m_action({ addChain(), addBody(), grabItem(), deleteItem(), bodyEdit() }),
    m_object(this) {
    setWorld(item);
}

void MapEditor::onInit() {
    Action::onInit();

    assert(world());

    ViewWorld* view = world()->view();
    m_focusedObject = view->focusedObject();
    view->setFocusedObject(nullptr);
    view->setFlickable(true);

    for (Action* action: m_action)
        action->setWorld(world());
}

void MapEditor::reset() {
    Action::reset();

    ViewWorld* view = world()->view();

    m_focusedObject->setFocus(true);
    view->setFocusedObject(m_focusedObject);
    view->setFlickable(false);
}
