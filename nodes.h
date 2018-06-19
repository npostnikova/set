#ifndef LIST_NODES_H
#define LIST_NODES_H

#include <cstdlib>

struct node_without_data {
    node_without_data *left;

    node_without_data *right;

    node_without_data *parent;

    node_without_data() : left(nullptr), right(nullptr), parent(nullptr) {}

    node_without_data(const node_without_data &other) : left(other.left), right(other.right), parent(other.parent) {}

    node_without_data(node_without_data *left, node_without_data *right, node_without_data *parent) :
            left(left), right(right), parent(parent){}

    bool has_parent() {
        return parent;
    }

    node_without_data*& get_parents_ref() {
        return (parent->left == this) ? parent->left : parent->right;
    }

    virtual void swap(node_without_data* f, node_without_data *sec) {
        node_without_data *s(sec);

        std::swap(this->left, s->left);
        std::swap(this->right, s->right);
        std::swap(this->parent, s->parent);
    }
};



template <typename T>
struct node : node_without_data {
    T key;

    node(const T &key, node_without_data *left, node_without_data *right, node_without_data *parent):
            node_without_data(left, right, parent), key(key) {}

    node(const T &key, node_without_data *parent) : node_without_data(nullptr, nullptr, parent), key(key) {}

    node(const T &key) : node_without_data(), key(key) {}

    node(const node& other) : key(other.key), node_without_data(static_cast<node_without_data&>(other)) {}

    ~node() {}

    virtual void swap(node_without_data *sec) {
        node *s = static_cast<node*>(sec);
        std::swap(this->left, s->left);
        std::swap(this->right, s->right);
        std::swap(this->parent, s->parent);
        std::swap(this->key, s->key);
    }
};

#endif //LIST_NODES_H
