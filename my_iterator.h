#ifndef LIST_MY_ITERATOR_H
#define LIST_MY_ITERATOR_H

#include "nodes.h"
#include <iterator>
#include <cstddef>

template <typename V>
struct my_iterator : std::iterator<std::bidirectional_iterator_tag, V, std::ptrdiff_t, V*, V&> {
    my_iterator() : cur_pos(nullptr), last_elem(nullptr) {}

    my_iterator(const my_iterator &it) : cur_pos(it.cur_pos), last_elem(it.last_elem) {}

    template <typename C, typename = typename std::enable_if<std::is_same<V, const C>::value>::type>
    my_iterator(const my_iterator<C> &other) : cur_pos(other.cur_pos), last_elem(other.last_elem) {}

    my_iterator& operator++() {
        cur_pos = next(cur_pos);
        return *this;
    }

    my_iterator operator++(int) {
        my_iterator result = *this;
        ++(*this);
        return result;
    }

    my_iterator& operator--() {
        cur_pos = prev(cur_pos);
        return *this;
    }

    my_iterator operator--(int) {
        my_iterator result = *this;
        --(*this);
        return result;
    }

    template <typename Z>
    bool operator==(my_iterator<Z> const &other) const {
        return cur_pos == other.cur_pos;
    }
    template <typename Z>
    bool operator!=(my_iterator<Z> const &other) const {
        return !(*this == other);
    }

    const V& operator*() const {
        return static_cast<const node<V>*>(cur_pos)->key;
    }

    const V* operator->() const {
        return &static_cast<const node<V>*>(cur_pos)->key;
    }
private:
    const node_without_data *cur_pos;

    const node_without_data *last_elem;

    explicit my_iterator(const node_without_data *cur_pos,
                         const node_without_data *last_elem) : cur_pos(cur_pos), last_elem(last_elem) {}

    const node_without_data *next(const node_without_data* cur_node) const;

    const node_without_data *prev(const node_without_data* cur_node);
    template <typename T>
    friend struct set;

    friend struct node_without_data;

    template <typename T>
    friend struct node;

    template <typename T>
    friend struct my_iterator;
};

template <typename T>
const node_without_data* my_iterator<T>::next(const node_without_data* cur_node) const {
    if (cur_node->right) {
        cur_node = cur_node->right;
        while (cur_node->left) {
            cur_node = cur_node->left;
        }
    } else {
        while (cur_node->parent != last_elem && cur_node->parent->right == cur_node) {
            cur_node = cur_node->parent;
        }
        if (cur_node->parent == last_elem) {
            return cur_node->parent;
        }
        cur_node = cur_node->parent;
    }
    return cur_node;
}

template <typename T>
const node_without_data* my_iterator<T>::prev(const node_without_data* cur_node) {
    if (cur_node->left != nullptr) {
        cur_node = (cur_node->left);
        while (cur_node->right)
            cur_node = cur_node->right;
    }
    else {
        while (cur_node->parent != last_elem && cur_node->parent->left == cur_node)
            cur_node = cur_node->parent;
        if (cur_node->parent == last_elem)
            return cur_node->parent;
        cur_node = cur_node->parent;
    }
    return cur_node;
}

#endif //LIST_MY_ITERATOR_H
