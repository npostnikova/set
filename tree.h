#ifndef SET_TREE_H
#define SET_TREE_H

#include <string>
#include <cstdlib>
#include <memory>
#include <iostream>
#include "nodes.h"

template <typename T>
struct tree {
    typedef node<T> node;
    tree() : last_elem(), root(nullptr) {}

    tree(const tree<T>& other);

    ~tree() noexcept {
        clear();
    }

    bool empty() const {
        return !root;
    }

    node_without_data* build_tree(node_without_data* parent, node_without_data *to_copy,
                                  tree &res_tree, const tree &to_copy_tree);

    node_without_data* find(T const &key) const;

    bool exists(T const &key) {
        return !empty() && find(key);
    }

    node_without_data* insert(T const &key);

    node_without_data * next(T const &val) const;

    node_without_data * prev(T const &val) const;

    const node_without_data *erase(T const &key);

    void clear() noexcept {
        clear(root);
        last_elem.parent = root = nullptr;
    }

    void clear(node_without_data* cur_node) noexcept;

    const node_without_data* min_node() const;

    template <typename Y>
    friend void swap(tree<Y> &first, tree<Y> &second) noexcept;
private:
    node_without_data last_elem;

    node_without_data *root;

    template <typename Z>
    friend struct set;
};

template <typename T>
tree<T>::tree(const tree<T>& other) : last_elem(), root(nullptr) {
    if (other.empty()) {
        return;
    }
    root = build_tree(root, other.root, *this, other);
}

template <typename T>
node_without_data* tree<T>::build_tree(node_without_data* parent, node_without_data *to_copy,
                              tree &res_tree, const tree &to_copy_tree) {
    if (!to_copy)
        return nullptr;
    node_without_data* res = new node(static_cast<node*>(to_copy)->key, parent);

    res->left = build_tree(res, to_copy->left, res_tree, to_copy_tree);
    if (to_copy->right == &to_copy_tree.last_elem) {
        res->right = &res_tree.last_elem;
        res_tree.last_elem.parent = res;
    } else {
        res->right = build_tree(res, to_copy->right, res_tree, to_copy_tree);
    }
    return res;
}

/* nothrow if T::operator</== doesn't throw */
template <typename T>
node_without_data* tree<T>::find(T const &key) const {
    node_without_data* tmp = root;
    while (tmp && tmp != &last_elem) {
        if (static_cast<node*>(tmp)->key < key)
            tmp = tmp->right;
        else if /* > */ (!(static_cast<node*>(tmp)->key < key)
                         && !(static_cast<node*>(tmp)->key == key))
            tmp = tmp->left;
        else
            return tmp;
    }
    return nullptr;
}

/* strong even if T::operator</== throws */
template <typename T>
node_without_data* tree<T>::insert(T const &key) {
    if (exists(key)) {
        return nullptr;
    }
    std::unique_ptr<node_without_data> new_node(new node(key));
    if (empty()) {
        last_elem.parent = root = new_node.release();
        root->right = &last_elem;
        return root;
    }
    node_without_data *tmp = root;
    while (true) {
        new_node->parent = tmp;
        if (static_cast<node*>(tmp)-> key < key) {
            if (!tmp->right)
                return tmp->right = new_node.release();
            if (tmp->right == &last_elem) {
                new_node->right = &last_elem;
                return last_elem.parent = tmp->right = new_node.release();
            }
            tmp = tmp->right;
        } else if /* > */(!(static_cast<node *>(tmp)->key < key)
                          && !(static_cast<node *>(tmp)->key == key)) {
            if (!tmp->left)
                return tmp->left = new_node.release();
            tmp = tmp->left;
        }
    }
}

template <typename T>
node_without_data * tree<T>::next(T const &val) const {
    node_without_data * current = root;
    node_without_data * successor = nullptr;

    while (current != nullptr && current != &last_elem) {
        if (static_cast<node*>(current)->key > val) {
            successor = current;
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return successor;
}

template <typename T>
node_without_data * tree<T>::prev(T const &val) const {
    node_without_data* current = root;
    node_without_data* successor = nullptr;

    while (current != nullptr && current != &last_elem) {
        if (static_cast<node*>(current)->key < val) {
            successor = current;
            current = current->right;
        } else {
            current = current->left;
        }
    }
    return successor;
}

template <typename T>
const node_without_data *tree<T>::erase(T const &key) {
    node_without_data *cur_node = find(key);
    if (!cur_node) /*doesn't exist*/ {
        return nullptr;
    }
    node_without_data *result = next(key);
    if (!cur_node->left && (!cur_node->right || cur_node->right == &last_elem)) {
        if (!cur_node->parent) {
            delete cur_node;
            return root = last_elem.parent = nullptr;
        }
        cur_node->get_parents_ref() = cur_node->right;
        last_elem.parent = cur_node->parent;
        delete cur_node;
    } else if (!cur_node->left ^ !cur_node->right) {
        node_without_data* child = cur_node->left ? cur_node->left : cur_node->right;
        if (cur_node->parent) {
            child->parent = cur_node->parent;
            cur_node->get_parents_ref() = child;
        } else {
            root = child;
        }
        delete cur_node;
    } else if (cur_node->left && cur_node->right == &last_elem) {
        cur_node->left->parent = cur_node->parent;
        cur_node->get_parents_ref() = cur_node->left;
        node_without_data* tmp = prev(key);
        tmp->right = &last_elem;
        last_elem.parent = tmp;
        delete cur_node;
    } else {
        auto del_key = static_cast<node*>(next(key))->key;
        erase(del_key);
        static_cast<node*>(cur_node)->key = del_key;
    }
    return result;
}


template <typename T>
void tree<T>::clear(node_without_data* cur_node) noexcept {
    if (cur_node) {
        clear(cur_node->left);
        if (cur_node->right != &last_elem)
            clear(cur_node->right);
        delete cur_node;
    }
}

template <typename T>
const node_without_data* tree<T>::min_node() const {
    node_without_data *tmp = root;
    while (tmp && tmp->left)
        tmp = tmp->left;
    return tmp;
}


template <typename Y>
void swap(tree<Y> &first, tree<Y> &second) noexcept {
    using std::swap;
    node_without_data *flp = first.root && first.root->left ? first.root->left->parent : nullptr;
    node_without_data *frp = first.root && first.root->right ? first.root->right->parent : nullptr;
    node_without_data *slp = second.root && second.root->left ? second.root->left->parent : nullptr;
    node_without_data *srp = second.root && second.root->right ? second.root->right->parent : nullptr;

    swap(flp, slp);
    swap(frp, srp);
    swap(first.root, second.root);

    if (first.last_elem.has_parent())
        first.last_elem.get_parents_ref() = &second.last_elem;
    if (second.last_elem.has_parent())
        second.last_elem.get_parents_ref() = &first.last_elem;
    swap(first.last_elem, second.last_elem);
}


#endif //SET_TREE_H
