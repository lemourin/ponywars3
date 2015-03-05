#include "ParticleSystem.hpp"

ParticleSystem::ParticleSystem(Item *parent): SceneGraph::Item(parent) {
}

ParticleSystem::~ParticleSystem() {
}

SceneGraph::Node *ParticleSystem::synchronize(SceneGraph::Node *) {
    return 0;
}


