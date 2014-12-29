#ifndef ADDPOLYGON_HPP
#define ADDPOLYGON_HPP
#include <vector>
#include "AddFixture.hpp"

class AddBody;

class AddPolygon: public AddFixture {
    private:
        Q_OBJECT

        std::vector<QPointF> m_pts;

    protected:
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        //QSGNode* updatePaintNode(QSGNode *, UpdatePaintNodeData *);

    public:
        AddPolygon(AddBody*);

        void reset();
        QFixture* fixture() const;

};

#endif // ADDPOLYGON_HPP
