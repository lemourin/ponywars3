#ifndef LIGHTSYSTEM_HPP
#define LIGHTSYSTEM_HPP

#include <QQuickItem>
#include <unordered_set>
#include "Graphics/ShaderSource.hpp"
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
        QSize m_resolution;

        EnlightedItems m_enlightedItems;

        ShaderSource m_normalMap;
        ShaderSource m_lightTexture;
        ShaderSource m_framebuffer[DYNAMIC_LIGHTS_COUNT];
        DynamicLight m_dynamicLight[DYNAMIC_LIGHTS_COUNT];

        std::vector<DynamicLight*> m_unusedLight;
        std::vector<StaticLight*> m_light;
        std::unordered_set<StaticLight*> m_visibleLights;

        void addLight(StaticLight*);
        void removeLight(StaticLight*);

        void initializeLight(StaticLight*);
        void initializeDynamicLights();

        void lightVisibilityChanged(StaticLight*);
        void visibleAreaChanged();

        void worldSizeChanged();

        static QQuickItem* createParticleSystem();

    protected:
        //QSGNode* updatePaintNode(QSGNode *, UpdatePaintNodeData *);
        void geometryChanged(const QRectF &newGeometry,
                             const QRectF &oldGeometry);

    public:
        explicit LightSystem(Game* game);
        ~LightSystem();

        void initialize();

        inline QSize resolution() const { return m_resolution; }
        void setResolution(QSize s);

        //inline QQuickItem* normalMapItem() const { return m_normalMap.shaderSourceItem(); }

        inline ShaderSource* normalMap() { return &m_normalMap; }
        inline ShaderSource* lightTexture() { return &m_lightTexture; }

        World* world() const;

        const std::unordered_set<StaticLight*>& visibleLights() const;

        void read(const QJsonObject&);
        void write(QJsonObject&) const;
};


#endif // LIGHTSYSTEM_HPP
