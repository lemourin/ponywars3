#ifndef BOX2DBOX_HPP
#define BOX2DBOX_HPP

#include "Fixture.hpp"
#include "Graphics/TexturedRectangle.hpp"

class Box2DBox: public Fixture {
    private:
        Q_OBJECT

        TexturedRectangle m_texturedRectangle;
        TexturedRectangle m_normalMapRectangle;

    protected:
        b2Shape* createShape() const;
        QSGNode* createNode() const;

    public:
        explicit Box2DBox(SceneGraph::Item* = nullptr);

        inline QRectF textureRect() const { return m_texturedRectangle.textureRect(); }
        void setTextureRect(QRectF);

        std::vector<QPointF> vertices() const;

        void setSize(QSizeF);

        bool read(const QJsonObject &);
        bool write(QJsonObject &) const;
};

#endif // BOX2DBOX_HPP
