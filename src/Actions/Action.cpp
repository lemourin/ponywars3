#include "Action.hpp"
#include "Utility/Utility.hpp"

Action::Action(SceneGraph::Item* parent):
    SceneGraph::Item(parent),
    m_world() {
}

void Action::reset() {
    //Utility::clearFocus(this);
}

void Action::focusChanged() {
    if (!focus())
        reset();
    else {
        onInit();
    }
}
