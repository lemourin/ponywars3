#include "Action.hpp"
#include "Utility/Utility.hpp"

Action::Action(SceneGraph::Item* parent):
    SceneGraph::Item(parent),
    m_world() {
    //setEnabled(false);
    //connect(this, &QQuickItem::focusChanged,
    //        this, &Action::focusChanged);
}

void Action::reset() {
    //Utility::clearFocus(this);
}

void Action::focusChanged() {
    //setEnabled(hasFocus());

    if (!hasFocus())
        reset();
    else {
        onInit();
        //forceActiveFocus();
    }
}
