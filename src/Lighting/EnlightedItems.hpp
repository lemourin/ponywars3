#ifndef ENLIGHTEDITEMS_HPP
#define ENLIGHTEDITEMS_HPP

#include <unordered_map>
#include "LightMaterial.hpp"
#include "Utility/Utility.hpp"
#include "SceneGraph/Item.hpp"
#include "SceneGraph/Geometry.hpp"

class LightSystem;
class QFixture;
class Light;
class World;

class EnlightedNode: public SceneGraph::TransformNode {
    private:
        struct Vertex { float x, y; };

        SceneGraph::GeometryNode m_geometryNode;
        SceneGraph::Geometry m_geometry;

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

    protected:
        class Node: public SceneGraph::Node {
            private:
                std::unordered_map<std::pair<QFixture*, Light*>, EnlightedNode*> m_data;

            public:
                Node();
                ~Node();

                SceneGraph::Node* getEnlightedNode(QFixture*, Light*);
        };

        SceneGraph::Node* synchronize(SceneGraph::Node* old);

    public:
        explicit EnlightedItems(LightSystem*, SceneGraph::Item*);

        inline LightSystem* lightSystem() const { return m_lightSystem; }
        World* world() const;

};

#endif // ENLIGHTEDITEMS_HPP
