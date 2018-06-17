#ifndef SET_TREE_H
#define SET_TREE_H

#include <string>
#include <cstdlib>
#include "nodes.h"

template <typename T>
struct tree {
    typedef node<const T> c_node;
    typedef node<T> node;
    node_without_data last_elem;

    node_without_data *root;

    tree() : last_elem(), root(nullptr) {}

    //template <typename C/*, typename = typename std::enable_if<std::is_same<T, const C>::value>::type*/>
    tree(const tree<T>& other) : last_elem(), root(nullptr) {
        //std::cout << "rrr" << std::endl;
        if (other.empty()) {
            return;
        }
        root = build_tree(root, other.root, *this, other);
        //std::cout << (other.root == nullptr);
    }
    bool empty() const {
        return !root;
    }

    node_without_data* build_tree(node_without_data* parent, node_without_data *to_copy, tree &res_tree, const tree &to_copy_tree) {
        node_without_data* res;
        if (to_copy) {
            res = new node(static_cast<node*>(to_copy)->key, parent);
        } else {
            return nullptr;
        }
        res->left = build_tree(res, to_copy->left, res_tree, to_copy_tree);
        if (to_copy->right == &to_copy_tree.last_elem) {
            res->right = &res_tree.last_elem;
            res_tree.last_elem.parent = res;
        } else {
            res->right = build_tree(res, to_copy->right, res_tree, to_copy_tree);
        }
        return res;
    }

    node_without_data* find(T const &key) const {
        node_without_data* tmp = root;
        while (tmp && tmp != &last_elem) {
            if (static_cast<node*>(tmp)->key < key) {
                tmp = tmp->right;
            } else if (!(static_cast<node*>(tmp)->key < key) && !(static_cast<node*>(tmp)->key == key)) {
                tmp = tmp->left;
            } else {
                return tmp;
            }
        }
        return nullptr;
    }

    bool exists(T key) {
        return !empty() && find(key);
    }

    node_without_data* insert(T key) {
        if (empty()) {
            root = new node(key);
            root->right = &last_elem;
            last_elem.parent = root;
        } else {
            if (exists(key)) {
                return nullptr;
            }
            node_without_data *tmp = root;
            while (true) {
                if (static_cast<node*>(tmp)->key < key) {
                    if (!tmp->right) {
                        tmp->right = new node(key, tmp);
                        return tmp->right;
                    } else if (tmp->right == &last_elem) {
                        tmp->right = new node(key, nullptr, &last_elem, tmp);
                        last_elem.parent = tmp->right;
                        return tmp->right;
                    } else {
                        tmp = tmp->right;
                    }
                } else if (!(static_cast<node*>(tmp)->key < key) && !(static_cast<node*>(tmp)->key == key)) {
                    if (!tmp->left) {
                        tmp->left = new node(key, tmp);
                        return tmp;
                    } else {
                        tmp = tmp->left;
                    }
                }
            }
        }
    }

    node_without_data * next(T const &val) const {
        node_without_data * current = root;
        node_without_data * successor = nullptr;

        while (current != nullptr) {
            if (static_cast<node*>(current)->key > val) {
                successor = current;
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return successor;
    }

    node_without_data * prev(T const &val) const {
        node_without_data* current = root;
        node_without_data* successor = nullptr;

        while (current != nullptr) {
            if (static_cast<node*>(current)->key < val) {
                successor = current;
                current = current->right;
            } else {
                current = current->left;
            }
        }
        return successor;
    }

    const node_without_data *erase(T key) {
        node_without_data *cur_node = find(key);
        if (!cur_node) /*doesn't exist*/ {
            return nullptr;
        }
        node_without_data *result = next(key);
        if (!cur_node->left && !cur_node->right) {
            if (cur_node->parent->left == cur_node) {
                cur_node->parent->left = nullptr;
            } else {
                cur_node->parent->right = nullptr;
            }
            delete cur_node;
        } else if(cur_node == root && !cur_node->left && cur_node->right == &last_elem) {
            delete root;
            return last_elem.parent = root = nullptr;
        } else if (!cur_node->left ^ !cur_node->right) {
            if (cur_node->right && cur_node->right == &last_elem) {
                if (cur_node->parent->left == cur_node) {
                    cur_node->parent->left = &last_elem;
                } else {
                    cur_node->parent->right = &last_elem;
                }
                last_elem.parent = cur_node->parent;
                delete cur_node;
                return result;
            }
            node_without_data* child = cur_node->left ? cur_node->left : cur_node->right;
            if (cur_node->parent) {
                child->parent = cur_node->parent;
                if (cur_node->parent->left == cur_node) {
                    cur_node->parent->left = child;
                } else {
                    cur_node->parent->right = child;
                }
            } else {
                root = child;
            }
            delete cur_node;
        } else if (cur_node->left && cur_node->right == &last_elem) {
            cur_node->left->parent = cur_node->parent;
            if (cur_node->parent->left == cur_node) {
                cur_node->parent->left = cur_node->left;
            } else {
                cur_node->parent->right = cur_node->left;
            }
            node_without_data* tmp = prev(key);
            tmp->right = &last_elem;
            last_elem.parent = tmp;
        } else {
            node_without_data* tmp = next(key);
            static_cast<node*>(cur_node)->key = static_cast<node*>(tmp)->key;
            if (tmp->parent->left == tmp) {
                tmp->parent->left = nullptr;
            } else {
                tmp->parent->right = nullptr;
            }
            delete tmp;
        }
        return result;
    }

    void clear() {
        clear(root);
        last_elem.parent = root = nullptr;
    }

    void clear(node_without_data* cur_node) {
        if (cur_node) {
            clear(cur_node->left);
            if (cur_node->right != &last_elem) {
                clear(cur_node->right);
            }
            delete cur_node;
        }
    }

    const node_without_data* min_node() const {
        node_without_data *tmp = root;
        if (tmp) {
            while (tmp->left) {
                tmp = tmp->left;
            }
        }
        return tmp;
    }

    template <typename Y>
    friend void swap(tree<Y> &first, tree<Y> &second);
};


template <typename Y>
void swap(tree<Y> &first, tree<Y> &second) {
    using std::swap;
    if (first.root && second.root) {
        swap(first.root->left->parent, second.root->left->parent);
        swap(second.root->right->parent, second.root->right->parent);
    }
    if (!first.root ^ !second.root) {
        if (!first.root) {
            second.root->left->parent = first.root;
            second.root->right->parent = first.root;
        } else {
            first.root->left->parent = second.root;
            first.root->right->parent = second.root;
        }
    }
    swap(first.root, second.root);
    node_without_data *t1, *t2;
    if (first.last_elem.parent) {
        if (first.last_elem.parent->left == &first.last_elem)
            first.last_elem.parent->left = &second.last_elem;
        else
            first.last_elem.parent->right = &second.last_elem;
    }
    if (second.last_elem.parent) {
        if (second.last_elem.parent->left == &second.last_elem)
            second.last_elem.parent->left = &first.last_elem;
        else
            second.last_elem.parent->right = &first.last_elem;
    }
    //swap(first.last_elem.parent, second.last_elem.parent);
    swap(first.last_elem, second.last_elem);
}


#endif //SET_TREE_H
