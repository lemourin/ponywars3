#include "Geometry.hpp"
#include <cassert>
#include <QDebug>

namespace SceneGraph {

Geometry::Geometry(std::vector<Attribute> set,
                   uint vertexCount,
                   uint vertexSize,
                   uint indexCount,
                   uint indexType):
    m_attribute(set),
    m_vertexCount(vertexCount),
    m_vertexSize(vertexSize),
    m_vertexData(malloc(vertexCount*vertexSize)),
    m_indexCount(indexCount),
    m_indexType(indexType),
    m_indexData(indexCount ? malloc(indexCount*sizeOfType(indexType)) : nullptr),
    m_drawingMode(GL_TRIANGLE_STRIP) {
    initializeOpenGLFunctions();
    glGenBuffers(1, &m_vbo);
}

Geometry::~Geometry() {
    glDeleteBuffers(1, &m_vbo);

    free(m_vertexData);
    free(m_indexData);
}

void Geometry::updateVertexData() {
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount()*vertexSize(),
                 vertexData(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Geometry::bind(const int* attributeLocation) {
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    uint id = 0, offset = 0;
    for (Attribute attribute: Geometry::attribute()) {
        glEnableVertexAttribArray(attributeLocation[id]);
        glVertexAttribPointer(attributeLocation[id],
                              attribute.tupleSize,
                              attribute.primitiveType,
                              GL_FALSE, vertexSize(), (void*)(size_t(offset)));
        id++;
        offset += attribute.tupleSize*sizeOfType(attribute.primitiveType);
    }
}

void Geometry::release() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

uint Geometry::sizeOfType(GLuint type) {
    if (type == GL_FLOAT)
        return sizeof(GLfloat);
    else if (type == GL_UNSIGNED_BYTE)
        return sizeof(GLubyte);
    else if (type == GL_UNSIGNED_SHORT)
        return sizeof(GLushort);
    else if (type == GL_UNSIGNED_INT)
        return sizeof(GLuint);
    else {
        assert(false);
        return 0;
    }
}

}
