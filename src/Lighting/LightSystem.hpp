#ifndef LIGHTSYSTEM_HPP
#define LIGHTSYSTEM_HPP

#include <QQuickItem>
#include <unordered_set>
#include "SceneGraph/ShaderSource.hpp"
#include "EnlightedItems.hpp"
#include "LightBlender.hpp"
#include "DynamicLight.hpp"

class StaticLight;
class DisplayItem;
class QSGDynamicTexture;
class World;
class Game;

class LightSystem: public SceneGraph::Item {
    private:
        friend class StaticLight;
        friend class DynamicLight;
        friend class World;
        friend class ViewWorld;

//        Q_PROPERTY(World* world READ world CONSTANT)
//        Q_PROPERTY(QQuickItem* normalMap READ normalMapItem CONSTANT)

        Game* m_game;
        QSizeF m_size;
        QSize m_resolution;

        SceneGraph::ShaderSource m_normalMap;
        SceneGraph::ShaderSource m_lightTexture;
        SceneGraph::ShaderSource m_framebuffer[DYNAMIC_LIGHTS_COUNT];
        DynamicLight m_dynamicLight[DYNAMIC_LIGHTS_COUNT];

        EnlightedItems m_enlightedItems;

        std::vector<DynamicLight*> m_unusedLight;
        std::vector<StaticLight*> m_light;
        std::unordered_set<StaticLight*> m_visibleLights;

        void addLight(StaticLight*);
        void removeLight(StaticLight*);

        void initializeDynamicLights();

        void lightVisibilityChanged(StaticLight*);
        void visibleAreaChanged();

        void addBody(QBody*);

    protected:
        SceneGraph::Node* synchronize(SceneGraph::Node *old);

    public:
        explicit LightSystem(Game* game);
        ~LightSystem();

        void initialize();

        inline QSizeF size() const { return m_size; }
        void setSize(QSizeF);

        inline QSize resolution() const { return m_resolution; }
        void setResolution(QSize s);

        inline SceneGraph::ShaderSource* normalMap() { return &m_normalMap; }
        inline SceneGraph::ShaderSource* lightTexture() { return &m_lightTexture; }

        World* world() const;

        const std::unordered_set<StaticLight*>& visibleLights() const;

        void read(const QJsonObject&);
        void write(QJsonObject&) const;
};


#endif // LIGHTSYSTEM_HPP
