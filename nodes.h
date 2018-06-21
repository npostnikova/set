#ifndef LIST_NODES_H
#define LIST_NODES_H

#include <cstdlib>
#include <algorithm>

template <typename Y>
struct tree;

struct node_without_data {
    node_without_data() : left(nullptr), right(nullptr), parent(nullptr) {}

    node_without_data(const node_without_data &other) : left(other.left), right(other.right), parent(other.parent) {}

    node_without_data(node_without_data *left, node_without_data *right, node_without_data *parent) :
            left(left), right(right), parent(parent) {}

    bool has_parent() {
        return parent;
    }

    node_without_data *&get_parents_ref() {
        return (parent->left == this) ? parent->left : parent->right;
    }

    friend void swap(node_without_data &f, node_without_data &s) noexcept;
protected:
    virtual void swap(node_without_data &s) noexcept {
        std::swap(this->left, s.left);
        std::swap(this->right, s.right);
        std::swap(this->parent, s.parent);
    }
    node_without_data *left;

    node_without_data *right;

    node_without_data *parent;

    template <typename Y>
    friend struct tree;

    template <typename Y>
    friend struct my_iterator;

    template <typename Y>
    friend void swap(tree<Y> &first, tree<Y> &second) noexcept;
};

void swap(node_without_data &f, node_without_data &s) noexcept {
    f.swap(s);
}

template <typename T>
struct node : node_without_data {
    node(const T &key, node_without_data *left, node_without_data *right, node_without_data *parent):
            node_without_data(left, right, parent), key(key) {}

    node(const T &key, node_without_data *parent) : node_without_data(nullptr, nullptr, parent), key(key) {}

    node(const T &key) : node_without_data(), key(key) {}

    node(const node& other) : key(other.key), node_without_data(static_cast<node_without_data&>(other)) {}

    ~node() {}

protected:
    virtual void swap(node &s) noexcept {
        this->node_without_data::swap(s);
        std::swap(this->key, s.key);
    }
private:
    T key;

    template <typename Y>
    friend struct tree;

    template <typename Y>
    friend struct my_iterator;
};

#endif //LIST_NODES_H
