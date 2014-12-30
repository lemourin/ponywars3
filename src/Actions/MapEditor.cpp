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
    m_action({ addChain(), addBody(), grabItem(), deleteItem(), bodyEdit() }) {
    //setFlag(ItemIsFocusScope);
    setWorld(item);
}

void MapEditor::actionFocusChanged(MapEditorAction* action) {
//    QString name = action->metaObject()->className();
//    if (action->hasFocus())
//        setCurrentAction(name);
//    else if (currentAction() == name)
//        setCurrentAction("");
}

void MapEditor::setCurrentAction(QString str) {
    if (m_currentAction == str)
        return;
    m_currentAction = str;
    //emit currentActionChanged();
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

    view->setFocusedObject(m_focusedObject);
    view->setFlickable(false);
}

void MapEditor::geometryChanged(const QRectF& newGeometry,
                                const QRectF& oldGeometry) {
    /*Action::geometryChanged(newGeometry, oldGeometry);

    for (Action* action: m_action) {
        action->setWidth(width());
        action->setHeight(height());
    }*/
}
