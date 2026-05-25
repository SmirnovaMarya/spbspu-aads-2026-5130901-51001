#ifndef BSTREE_IMPL_HPP
#define BSTREE_IMPL_HPP

#include "bstree.hpp"
#include <memory>

namespace smirnova
{
  template< class NodeT >
  TreeNodeBase* makeFakeLeaf()
  {
    auto tmp = std::make_unique<NodeT>();

    tmp->parent_ = tmp.get();
    tmp->left_ = tmp.get();
    tmp->right_ = tmp.get();

    return tmp.release();
  }

  TreeNodeBase* getMin(TreeNodeBase* node, TreeNodeBase* fake)
  {
    while (node->left_ != fake)
    {
      node = node->left_;
    }

    return node;
  }

  template< class Key, class Value >
  void BSTree<Key, Value>::swap(BSTree& other) noexcept
  {
    std::swap(root_, other.root_);
    std::swap(fake_leaf_, other.fake_leaf_);
    std::swap(size_, other.size_);
  }

  template< class Key, class Value >
  typename BSTree<Key, Value>::iterator BSTree<Key, Value>::begin() noexcept
  {
    if (!root_)
    {
      return end();
    }

    return iterator(
      getMin(root_, fake_leaf_),
      fake_leaf_
    );
  }

  template< class Key, class Value >
  typename BSTree<Key, Value>::iterator BSTree<Key, Value>::end() noexcept
  {
    return iterator(fake_leaf_, fake_leaf_);
  }

  template< class Key, class Value >
  typename BSTree<Key, Value>::const_iterator BSTree<Key, Value>::begin() const noexcept
  {
    return cbegin();
  }

  template< class Key, class Value >
  typename BSTree<Key, Value>::const_iterator BSTree<Key, Value>::end() const noexcept
  {
    return cend();
  }

  template< class Key, class Value >
  typename BSTree<Key, Value>::const_iterator BSTree<Key, Value>::cbegin() const noexcept
  {
    if (!root_)
    {
      return cend();
    }

    return const_iterator(
      getMin(root_, fake_leaf_),
      fake_leaf_
    );
  }

  template< class Key, class Value >
  typename BSTree<Key, Value>::const_iterator BSTree<Key, Value>::cend() const noexcept
  {
    return const_iterator(fake_leaf_, fake_leaf_);
  }

  template< class Key, class Value >
  bool BSTree<Key, Value>::contains(const Key& key) const noexcept
  {
    return findNode(key) != nullptr;
  }

  template< class Key, class Value >
  void BSTree<Key, Value>::push(const Key& key, const Value& value)
  {
    if (!root_)
    {
      auto tmp = std::make_unique<typename BSTree<Key, Value>::NodeT>(key, value, fake_leaf_);

      tmp->left_ = fake_leaf_;
      tmp->right_ = fake_leaf_;

      root_ = tmp.release();

      ++size_;
      return;
    }

    TreeNodeBase* node = root_;
    TreeNodeBase* parent = fake_leaf_;

    while (node != fake_leaf_)
    {
      parent = node;

      typename BSTree<Key, Value>::NodeT* n = static_cast< typename BSTree<Key, Value>::NodeT* >(node);

      if (key < n->data_.first)
      {
        node = node->left_;
      }
      else if (n->data_.first < key)
      {
        node = node->right_;
      }
      else
      {
        n->data_.second = value;
        return;
      }
    }

    auto tmp = std::make_unique<typename BSTree<Key, Value>::NodeT>(key, value, parent);

    tmp->left_ = fake_leaf_;
    tmp->right_ = fake_leaf_;

    typename BSTree<Key, Value>::NodeT* p = static_cast< typename BSTree<Key, Value>::NodeT* >(parent);

    if (key < p->data_.first)
    {
      parent->left_ = tmp.release();
    }
    else
    {
      parent->right_ = tmp.release();
    }

    ++size_;
  }

  template< class Key, class Value >
  Value& BSTree<Key, Value>::get(const Key& key)
  {
    typename BSTree<Key, Value>::NodeT* node = findNode(key);

    if (!node)
    {
      throw std::runtime_error("key not found");
    }

    return node->data_.second;
  }

  template< class Key, class Value >
  const Value& BSTree<Key, Value>::get(const Key& key) const
  {
    typename BSTree<Key, Value>::NodeT* node = findNode(key);

    if (!node)
    {
      throw std::runtime_error("key not found");
    }

    return node->data_.second;
  }

  template< class Key, class Value >
  Value BSTree<Key, Value>::drop(const Key& key)
  {
    typename BSTree<Key, Value>::NodeT* node = findNode(key);

    if (!node)
    {
      throw std::runtime_error("key not found");
    }

    Value result = node->data_.second;

    if (node->left_ == fake_leaf_)
    {
      transplant(node, node->right_);
    }
    else if (node->right_ == fake_leaf_)
    {
      transplant(node, node->left_);
    }
    else
    {
      typename BSTree<Key, Value>::NodeT* next = static_cast< typename BSTree<Key, Value>::NodeT* >(getMin(node->right_, fake_leaf_));

      if (next->parent_ != node)
      {
        transplant(next, next->right_);

        next->right_ = node->right_;
        next->right_->parent_ = next;
      }

      transplant(node, next);

      next->left_ = node->left_;
      next->left_->parent_ = next;
    }

    delete node;

    --size_;

    if (size_ == 0)
    {
      root_ = nullptr;
    }

    return result;
  }

  template< class Key, class Value >
  size_t BSTree<Key, Value>::height() const
  {
    return height(root_);
  }

  template< class Key, class Value >
  size_t BSTree<Key, Value>::height(const_iterator it) const
  {
    return height(
      const_cast< TreeNodeBase* >(it.current_)
    );
  }

  template< class Key, class Value >
  typename BSTree<Key, Value>::iterator BSTree<Key, Value>::rotateLeft(iterator it)
  {
    return iterator(rotateLeftNode(it.current_), fake_leaf_);
  }

  template< class Key, class Value >
  typename BSTree<Key, Value>::iterator BSTree<Key, Value>::rotateRight(iterator it)
  {
    return iterator(rotateRightNode(it.current_), fake_leaf_);
  }

  template< class Key, class Value >
  typename BSTree<Key, Value>::iterator BSTree<Key, Value>::rotateLargeLeft(iterator it)
  {
    return iterator(rotateLargeLeftNode(it.current_), fake_leaf_);
  }

  template< class Key, class Value >
  typename BSTree<Key, Value>::iterator BSTree<Key, Value>::rotateLargeRight(iterator it)
  {
    return iterator(rotateLargeRightNode(it.current_), fake_leaf_);
  }

  template< class Key, class Value >
  void BSTree<Key, Value>::clear(TreeNodeBase* node) noexcept
  {
    if (!node || node == fake_leaf_)
    {
      return;
    }

    clear(node->left_);
    clear(node->right_);

    delete static_cast< typename BSTree<Key, Value>::NodeT* >(node);
  }

  template< class Key, class Value >
  TreeNodeBase* BSTree<Key, Value>::copy(
    TreeNodeBase* other,
    TreeNodeBase* parent,
    TreeNodeBase* otherFake
  )
  {
    if (!other || other == otherFake)
    {
      return fake_leaf_;
    }

    typename BSTree<Key, Value>::NodeT* otherNode = static_cast< typename BSTree<Key, Value>::NodeT* >(other);

    auto tmp = std::make_unique<typename BSTree<Key, Value>::NodeT>(otherNode->data_.first, otherNode->data_.second, parent);

    tmp->left_ = copy(other->left_, tmp.get(), otherFake);

    tmp->right_ = copy(other->right_, tmp.get(), otherFake);

    return tmp.release();
  }

  template< class Key, class Value >
  typename BSTree<Key, Value>::NodeT* BSTree<Key, Value>::findNode(const Key& key) const noexcept
  {
    TreeNodeBase* node = root_;

    while (node && node != fake_leaf_)
    {
      typename BSTree<Key, Value>::NodeT* n = static_cast< typename BSTree<Key, Value>::NodeT* >(node);

      if (key < n->data_.first)
      {
        node = node->left_;
      }
      else if (n->data_.first < key)
      {
        node = node->right_;
      }
      else
      {
        return n;
      }
    }

    return nullptr;
  }

  template< class Key, class Value >
  size_t BSTree<Key, Value>::height(TreeNodeBase* node) const noexcept
  {
    if (!node || node == fake_leaf_)
    {
      return 0;
    }

    size_t left = height(node->left_);
    size_t right = height(node->right_);

    return (left > right ? left : right) + 1;
  }

  template< class Key, class Value >
  void BSTree<Key, Value>::transplant(TreeNodeBase* u, TreeNodeBase* v)
  {
    if (u->parent_ == fake_leaf_)
    {
      root_ = (v == fake_leaf_) ? nullptr : v;
    }
    else if (u == u->parent_->left_)
    {
      u->parent_->left_ = v;
    }
    else
    {
      u->parent_->right_ = v;
    }

    if (v != fake_leaf_)
    {
      v->parent_ = u->parent_;
    }
  }

  template< class Key, class Value >
  TreeNodeBase* BSTree<Key, Value>::rotateLeftNode(TreeNodeBase* x)
  {
    if (!x || x == fake_leaf_)
      return x;

    TreeNodeBase* y = x->right_;
    if (!y || y == fake_leaf_)
      return x;

    transplant(x, y);

    x->right_ = y->left_;
    if (y->left_ != fake_leaf_)
      y->left_->parent_ = x;

    y->left_ = x;
    x->parent_ = y;

    return y;
  }

  template< class Key, class Value >
  TreeNodeBase* BSTree<Key, Value>::rotateRightNode(TreeNodeBase* x)
  {
    if (!x || x == fake_leaf_)
      return x;

    TreeNodeBase* y = x->left_;
    if (!y || y == fake_leaf_)
      return x;

    transplant(x, y);

    x->left_ = y->right_;
    if (y->right_ != fake_leaf_)
      y->right_->parent_ = x;

    y->right_ = x;
    x->parent_ = y;

    return y;
  }

  template< class Key, class Value >
  TreeNodeBase* BSTree<Key, Value>::rotateLargeLeftNode(TreeNodeBase* x)
  {
    if (!x || x == fake_leaf_)
    {
      return x;
    }
    if (x->right_ == fake_leaf_)
    {
      return x;
    }
    rotateRightNode(x->right_);
    return rotateLeftNode(x);
  }

  template< class Key, class Value >
  TreeNodeBase* BSTree<Key, Value>::rotateLargeRightNode(TreeNodeBase* x)
  {
    if (!x || x == fake_leaf_)
    {
      return x;
    }
    if (x->left_ == fake_leaf_)
    {
      return x;
    }
    rotateLeftNode(x->left_);
    return rotateRightNode(x);
  }

}

#endif
