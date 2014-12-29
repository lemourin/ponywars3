#include "Node.hpp"

namespace SceneGraph {

Node::Node(Node* parent, Type type):
    BaseObject(parent),
    m_type(type) {
}

Node* Node::firstChild() const {
    return static_cast<Node*>(BaseObject::firstChild());
}

Node* Node::next() const {
    return static_cast<Node*>(BaseObject::next());
}

Node* Node::parent() const {
    return static_cast<Node*>(BaseObject::parent());
}

void Node::appendChild(Node* node) {
    BaseObject::appendChild(node);
}

void Node::removeChild(Node* node) {
    BaseObject::removeChild(node);
}

GeometryNode::GeometryNode(Node* parent):
    Node(parent, Type::GeometryNode),
    m_material(),
    m_geometry() {
}

TransformNode::TransformNode(Node* parent):
    Node(parent, Type::TransformNode) {
}

}
