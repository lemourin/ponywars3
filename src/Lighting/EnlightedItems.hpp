#ifndef ENLIGHTEDITEMS_HPP
#define ENLIGHTEDITEMS_HPP

#include <QSGNode>
#include <unordered_map>
#include "LightMaterial.hpp"
#include "Utility/Utility.hpp"
#include "SceneGraph/Item.hpp"

class LightSystem;
class QFixture;
class Light;
class World;

class EnlightedNode: public QSGTransformNode {
    private:
        QSGGeometryNode m_geometryNode;
        QSGGeometry m_geometry;

        LightMaterial m_material;

        void updateMaterial(Light*);
        void updateMatrix(QFixture*);

    public:
        EnlightedNode(QFixture*);

        void update(QFixture*, Light*);
};

class EnlightedItems: public SceneGraph::Item {
    private:        
        LightSystem* m_lightSystem;
        World* m_world;

    protected:
        class Node: public QSGNode {
            private:
                std::unordered_map<std::pair<QFixture*, Light*>, EnlightedNode*> m_data;

            public:
                Node();
                ~Node();

                QSGNode* getEnlightedNode(QFixture* fixture, Light* light);
        };

        //QSGNode* updatePaintNode(QSGNode*, UpdatePaintNodeData*);

    public:
        explicit EnlightedItems(SceneGraph::Item* = nullptr);

        inline LightSystem* lightSystem() const { return m_lightSystem; }
        inline void setLightSystem(LightSystem* l) { m_lightSystem = l; }

        inline World* world() const { return m_world; }
        inline void setWorld(World* i) { m_world = i; }


};

#endif // ENLIGHTEDITEMS_HPP
