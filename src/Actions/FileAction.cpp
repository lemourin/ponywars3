#include "FileAction.hpp"
#include "MainAction.hpp"

FileAction::FileAction(MainAction* action):
    SubAction(action),
    m_object(this) {
    setWorld(action->world());
}

FileAction::~FileAction() {
}

