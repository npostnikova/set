#ifndef SET_SET_H
#define SET_SET_H

#include "tree.h"
#include "my_iterator.h"
#include <utility>

template <typename T>
struct set {
    typedef my_iterator<const T> const_iterator;
    typedef const_iterator iterator;
    typedef std::reverse_iterator<const_iterator> reverse_const_iterator;
    typedef reverse_const_iterator reverse_iterator;

    set();

    set(set const& other);

    ~set() noexcept;

    set& operator=(const set& other);

    template <typename Y>
    friend void swap(set<Y>& first, set<Y>& second) noexcept;

    std::pair<const_iterator, bool> insert(T const& key);

    const_iterator erase(const_iterator pos);

    const_iterator find(T const& key) const;

    const_iterator lower_bound(T const& key) const;

    const_iterator upper_bound(T const& key) const;

    bool empty() const noexcept;

    void clear() noexcept;

    const_iterator begin() const;

    const_iterator end() const;

    reverse_const_iterator rbegin() const;

    reverse_const_iterator rend() const;

private:
    tree<T> set_tree;
};

template <typename T>
set<T>::set() {}

template <typename T>
set<T>::set(set<T> const &other)
    try: set_tree(other.set_tree) {} catch (...) { clear(); }

template <typename T>
set<T>::~set() noexcept {
    clear();
}

template <typename T>
set<T>& set<T>::operator=(const set<T>& other) {
    set tmp(other);
    swap(*this, tmp);
    return *this;
}

template <typename T>
void swap(set<T> &first, set<T> &second) noexcept {
    swap(first.set_tree, second.set_tree);
}

template <typename T>
std::pair<typename set<T>::const_iterator, bool> set<T>::insert(T const &key) {
    node_without_data *result = set_tree.insert(key);
    return {const_iterator(result, &set_tree.last_elem),
             result != nullptr};
}

template <typename T>
typename set<T>::const_iterator
set<T>::erase(const_iterator pos) {
    if (auto tmp = set_tree.erase(*pos))
        return iterator(tmp, &set_tree.last_elem);
    return end();
}

template <typename T>
typename set<T>::const_iterator set<T>::find(T const &key) const {
    auto res = set_tree.find(key);
    return res ? iterator(set_tree.find(key), &set_tree.last_elem) : end();
}

template <typename T>
typename set<T>::const_iterator set<T>::lower_bound(T const &key) const {
    const node_without_data * tmp = nullptr;
    if ((tmp = set_tree.find(key)) || (tmp = set_tree.next(key))) {
        return iterator(tmp, &set_tree.last_elem);
    }
    return end();
}

template <typename T>
typename set<T>::const_iterator set<T>::upper_bound(T const &key) const {
    const node_without_data * tmp = set_tree.next(key);
    if (!tmp)
        return end();
    return iterator(set_tree.next(key), &set_tree.last_elem);
}


template <typename T>
bool set<T>::empty() const noexcept {
    return set_tree.empty();
}

template <typename T>
void set<T>::clear() noexcept {
    set_tree.clear();
}

template <typename T>
typename set<T>::const_iterator set<T>::begin() const {
    if (empty()) return end();
    return iterator(set_tree.min_node(), &set_tree.last_elem);
}

template <typename T>
typename set<T>::const_iterator set<T>::end() const {
    return iterator(&set_tree.last_elem, &set_tree.last_elem);
}

template <typename T>
typename set<T>::reverse_const_iterator set<T>::rbegin() const {
    return reverse_const_iterator(end());
}

template <typename T>
typename set<T>::reverse_const_iterator set<T>::rend() const {
    return reverse_const_iterator(begin());
}


#endif //SET_SET_H
