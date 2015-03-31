#ifndef MAINACTION_HPP
#define MAINACTION_HPP
#include "Action.hpp"
#include "MapEditor.hpp"
#include "FileAction.hpp"

class World;

class MainAction: public Action {
    private:
        friend class World;

        BaseItem* m_focusedObject;

        MapEditor m_mapEditor;
        FileAction m_fileAction;

    protected:
        void onInit();
        void reset();

        void focusedObjectDestroyed();

    public:
        MainAction(World*);
        ~MainAction();

        inline MapEditor* mapEditor() { return &m_mapEditor; }
        inline FileAction* fileAction() { return &m_fileAction; }
};

#endif // MAINACTION_HPP
