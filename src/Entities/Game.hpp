#ifndef GAME_HPP
#define GAME_HPP

#include <QQuickItem>
#include "Entities/ViewWorld.hpp"
#include "Lighting/LightSystem.hpp"

class Game: public DisplayItemFrame {
    private:
        ViewWorld m_viewWorld;
        LightSystem m_lightSystem;

    protected:
        void geometryChanged(const QRectF &newGeometry,
                             const QRectF &oldGeometry);

    public:
        explicit Game(SceneGraph::Item *parent = 0);

        inline ViewWorld* view() { return &m_viewWorld; }
        inline const ViewWorld* view() const { return &m_viewWorld; }

        inline LightSystem* lightSystem() { return &m_lightSystem; }
        inline const LightSystem* lightSystem() const { return &m_lightSystem; }

        bool read(const QJsonObject& obj);
        bool write(QJsonObject& obj) const;

        Q_INVOKABLE void load(QString path);
        Q_INVOKABLE void dump(QString path) const;

};

#endif // GAME_HPP
