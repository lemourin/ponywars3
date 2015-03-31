#include "MainAction.hpp"
#include "Entities/World.hpp"
#include "Entities/ViewWorld.hpp"

MainAction::MainAction(World* w):
    Action(w, w),
    m_focusedObject(),
    m_mapEditor(this),
    m_fileAction(this) {
}

MainAction::~MainAction() {
}

void MainAction::onInit() {
    Action::onInit();

    assert(world());

    ViewWorld* view = world()->view();
    m_focusedObject = view->focusedObject();
    view->setFocusedObject(nullptr);
    view->setFlickable(true);
}

void MainAction::reset() {
    Action::reset();

    ViewWorld* view = world()->view();

    if (m_focusedObject)
        m_focusedObject->setFocus(true);
    else
        world()->setFocus(true);

    view->setFocusedObject(m_focusedObject);
    view->setFlickable(false);
}

void MainAction::focusedObjectDestroyed() {
    m_focusedObject = nullptr;
}


