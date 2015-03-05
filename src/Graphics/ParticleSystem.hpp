#ifndef PARTICLESYSTEM_HPP
#define PARTICLESYSTEM_HPP
#include "SceneGraph/Item.hpp"
#include "SceneGraph/Node.hpp"

struct Particle {

};

class ParticleSystem: public SceneGraph::Item {
    private:
        class Node: public SceneGraph::GeometryNode {
            private:
            public:
        };

    protected:
        SceneGraph::Node* synchronize(SceneGraph::Node*);

    public:
        ParticleSystem(SceneGraph::Item* = nullptr);
        ~ParticleSystem();
};

#endif // PARTICLESYSTEM_HPP
