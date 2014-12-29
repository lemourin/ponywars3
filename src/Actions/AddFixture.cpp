#include "AddFixture.hpp"
#include "AddBody.hpp"

AddFixture::AddFixture(AddBody* parent):
    Action(parent),
    m_addBody(parent) {
}

void AddFixture::finished() {
    m_addBody->finishedSubaction(this);
}

void AddFixture::focusChanged() {
    Action::focusChanged();
    m_addBody->actionFocusChanged(this);
}



