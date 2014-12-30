#ifndef MAPEDITOR_HPP
#define MAPEDITOR_HPP
#include "AddChain.hpp"
#include "AddBody.hpp"
#include "GrabItem.hpp"
#include "DeleteItem.hpp"
#include "BodyEdit.hpp"
#include "Utility/BaseItem.hpp"
#include <QPointer>

class MapEditorAction;

class MapEditor: public Action {
    private:
        friend class MapEditorAction;

        /*Q_PROPERTY(AddChain* addChain READ addChain CONSTANT)
        Q_PROPERTY(AddBody* addBody READ addBody CONSTANT)
        Q_PROPERTY(GrabItem* grabItem READ grabItem CONSTANT)
        Q_PROPERTY(DeleteItem* deleteItem READ deleteItem CONSTANT)
        Q_PROPERTY(BodyEdit* bodyEdit READ bodyEdit CONSTANT)
        Q_PROPERTY(QString currentAction READ currentAction NOTIFY currentActionChanged)*/

        AddChain m_addChain;
        AddBody m_addBody;
        GrabItem m_grabItem;
        DeleteItem m_deleteItem;
        BodyEdit m_bodyEdit;
        QString m_currentAction;

        std::vector<Action*> m_action;
        BaseItem* m_focusedObject;

        void actionFocusChanged(MapEditorAction*);
        void setCurrentAction(QString);

    protected:
        void onInit();
        void reset();

        void geometryChanged(const QRectF &newGeometry,
                             const QRectF &oldGeometry);

    public:
        MapEditor(World*);

        inline AddChain* addChain() { return &m_addChain; }
        inline AddBody* addBody() { return &m_addBody; }
        inline GrabItem* grabItem() { return &m_grabItem; }
        inline DeleteItem* deleteItem() { return &m_deleteItem; }
        inline BodyEdit* bodyEdit() { return &m_bodyEdit; }

        inline QString currentAction() const { return m_currentAction; }

    //signals:
        void currentActionChanged();
};

#endif // MAPEDITOR_HPP
