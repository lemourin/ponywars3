#ifndef SPRITESEQUENCE_HPP
#define SPRITESEQUENCE_HPP

#include <unordered_map>
#include "SceneGraph/Item.hpp"
#include "Graphics/TexturedRectangle.hpp"

struct SpriteData {
    std::string m_image;
    std::string m_name;
    std::string m_next;
    uint m_frameCount;
    uint m_frameRate;
    qreal m_frameX;
    qreal m_frameY;
    qreal m_frameWidth;
    qreal m_frameHeight;
};

class SpriteSequence: public SceneGraph::Item {
    private:
        TexturedRectangle m_texture;
        std::unordered_map< std::string, SpriteData > m_data;
        SpriteData* m_currentSprite;
        uint m_currentFrame;
        int m_timerId;

        void visibleChanged();

    protected:
        virtual void currentSpriteChanged();
        void timerEvent(QTimerEvent *);
        void geometryChanged(const QRectF &newGeometry,
                             const QRectF &oldGeometry);
        //void itemChange(ItemChange, const ItemChangeData &);


    public:
        explicit SpriteSequence(SceneGraph::Item* = nullptr);
        void reset();

        void addSprite(const SpriteData& data);
        void setFrame(const SpriteData* data, uint frame);

        const char* currentSprite() const;
        void setCurrentSprite(const char*);

        inline void setMatrix(const QMatrix4x4& m) { m_texture.setMatrix(m); }
        inline const QMatrix4x4& matrix() const { return m_texture.matrix(); }
};

#endif // SPRITESEQUENCE_HPP
