#ifndef WORLD_HPP
#define WORLD_HPP

#include <QElapsedTimer>

#include "QBox2D/QWorld.hpp"
#include "QBox2D/QBody.hpp"
#include "QBox2D/Fixture/Box2DBox.hpp"

#include "Actions/MapEditor.hpp"

#include "ItemSet.hpp"

class Game;
class Player;
class LightSystem;
class ViewWorld;

class Boundary: public QBody {
    private:
        Box2DBox m_left;
        Box2DBox m_right;
        Box2DBox m_top;
        Box2DBox m_bottom;

    public:
        Boundary(qreal width,
                 qreal height,
                 qreal thickness,
                 SceneGraph::Item* = nullptr);
};

class World: public QWorld {
    private:
        friend class ViewWorld;

//        Q_PROPERTY(Player* player READ player WRITE setPlayer NOTIFY playerChanged)
//        Q_PROPERTY(qreal fps READ fps WRITE setFps NOTIFY fpsChanged)
//        Q_PROPERTY(MapEditor* mapEditor READ mapEditor CONSTANT)
//        Q_PROPERTY(bool paused READ paused WRITE setPaused NOTIFY pausedChanged)

        ViewWorld* m_viewWorld;

        Player* m_player;
        qreal m_fps;

        QElapsedTimer m_fpscounter;
        Boundary* m_boundary;

        ItemSet m_itemSet;
        MapEditor m_mapEditor;

        void updateFps();

    protected:
        void mousePressEvent(QMouseEvent*);
        void geometryChanged(const QRectF &newGeometry,
                             const QRectF &oldGeometry);
        //void itemChange(ItemChange, const ItemChangeData &);
        void onBodyDestroyed(QBody*);
        void onBodyAdded(QBody*);

    public:
        explicit World(ViewWorld*);

        inline Player* player() const { return m_player; }
        void setPlayer(Player* p);

        inline ViewWorld* view() const { return m_viewWorld; }
        LightSystem* lightSystem() const;

        inline qreal fps() const { return m_fps; }
        void setFps(qreal);

        inline bool paused() const { return !isRunning(); }
        void setPaused(bool);

        inline MapEditor* mapEditor() { return &m_mapEditor; }

        void read(const QJsonObject&);
        void write(QJsonObject&) const;

//    signals:
//        void playerChanged();
//        void fpsChanged();
//        void pausedChanged();
//        void gameOver();
//        void explosion(QPointF pos);
};

#endif // WORLD_HPP
