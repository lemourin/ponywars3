#ifndef FIXTURE_HPP
#define FIXTURE_HPP

#include "QBox2D/QFixture.hpp"

class TexturedItem;

class Fixture: public QFixture {
    private:
        TexturedItem* m_texture;
        TexturedItem* m_normalMap;

        void updatePosition();
        void updateRotation();
        void updateSize();
        void updateVisibility();

    protected:
        void initialize();
        void geometryChanged(const QRectF &newGeometry,
                             const QRectF &oldGeometry);
        //void itemChange(ItemChange, const ItemChangeData &);
        void bodyPositionChanged();
        void bodyRotationChanged();

    public:
        explicit Fixture(QQuickItem* = nullptr);
        ~Fixture();

        QString textureSource() const;
        void setTextureSource(QString);

        QString normalMapSource() const;
        void setNormalMapSource(QString);

        inline TexturedItem* texture() const { return m_texture; }
        inline void setTexture(TexturedItem* i) { m_texture = i; }

        inline TexturedItem* normalMap() const { return m_normalMap; }
        inline void setNormalMap(TexturedItem* i) { m_normalMap = i; }

        bool read(const QJsonObject &);
        bool write(QJsonObject &) const;

};

#endif // FIXTURE_HPP
