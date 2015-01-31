#ifndef MAPEDITOR_HPP
#define MAPEDITOR_HPP
#include "AddChain.hpp"
#include "AddBody.hpp"
#include "GrabItem.hpp"
#include "DeleteItem.hpp"
#include "BodyEdit.hpp"
#include "Utility/BaseItem.hpp"
#include <QPointer>

class SubAction;
class MapEditor;
class AddBodyObject;

class MapEditor: public Action {
    private:
        friend class SubAction;
        friend class MapEditorObject;

        AddChain m_addChain;
        AddBody m_addBody;
        GrabItem m_grabItem;
        DeleteItem m_deleteItem;
        BodyEdit m_bodyEdit;

        std::vector<Action*> m_action;
        BaseItem* m_focusedObject;

        ActionObject m_object;

        void actionEnabledChanged(SubAction*);
        void setCurrentAction(QString);

    protected:
        void onInit();
        void reset();

    public:
        MapEditor(World*);

        inline AddChain* addChain() { return &m_addChain; }
        inline AddBody* addBody() { return &m_addBody; }
        inline GrabItem* grabItem() { return &m_grabItem; }
        inline DeleteItem* deleteItem() { return &m_deleteItem; }
        inline BodyEdit* bodyEdit() { return &m_bodyEdit; }
};

#endif // MAPEDITOR_HPP
