#ifndef ACTION_HPP
#define ACTION_HPP
#include "SceneGraph/Item.hpp"

class World;

class Action: public SceneGraph::Item {
    private:
        World* m_world;

    protected:
        inline virtual void onInit() { }
        virtual void focusChanged();

    public:
        Action(SceneGraph::Item* = nullptr);

        virtual void reset();

        inline World* world() const { return m_world; }
        inline void setWorld(World* w) { m_world = w; }

        inline bool hasFocus() const { return false; }

        inline virtual void finished() { }

};

#endif // ACTION_HPP
