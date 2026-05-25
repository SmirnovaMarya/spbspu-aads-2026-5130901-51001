#ifndef TREEITERATORS_HPP
#define TREEITERATORS_HPP

#include <stdexcept>
#include <utility>

namespace smirnova
{
  template< class Key, class Value >
  struct Node;

  template< class NodeT >
  NodeT* nextNode(NodeT* node, NodeT* fake)
  {
    if (node->right_ != fake)
    {
      node = node->right_;

      while (node->left_ != fake)
      {
        node = node->left_;
      }

      return node;
    }

    NodeT* parent = node->parent_;

    while (parent != fake && node == parent->right_)
    {
      node = parent;
      parent = parent->parent_;
    }

    return parent;
  }

  template< class Key, class Value >
  class BSTConstIterator;

  template< class Key, class Value >
  class BSTIterator
  {
  public:
    using NodeT = Node< Key, Value >;

    BSTIterator():
      current_(nullptr),
      fake_(nullptr)
    {}

    BSTIterator(NodeT* current, NodeT* fake):
      current_(current),
      fake_(fake)
    {}

    std::pair< Key, Value >& operator*() const
    {
      if (current_ == fake_)
      {
        throw std::runtime_error("bad iterator");
      }

      return current_->data_;
    }

    std::pair< Key, Value >* operator->() const
    {
      return &current_->data_;
    }

    BSTIterator& operator++()
    {
      if (current_ != fake_)
      {
        current_ = nextNode(current_, fake_);
      }

      return *this;
    }

    BSTIterator operator++(int)
    {
      BSTIterator temp(*this);
      ++(*this);
      return temp;
    }

    bool operator==(const BSTIterator& other) const noexcept
    {
      return current_ == other.current_;
    }

    bool operator!=(const BSTIterator& other) const noexcept
    {
      return !(*this == other);
    }

  private:
    NodeT* current_;
    NodeT* fake_;

    template< class K, class V >
    friend class BSTConstIterator;

    template< class K, class V >
    friend class BSTree;
  };

  template< class Key, class Value >
  class BSTConstIterator
  {
  public:
    using NodeT = Node< Key, Value >;

    BSTConstIterator():
      current_(nullptr),
      fake_(nullptr)
    {}

    BSTConstIterator(const NodeT* current, const NodeT* fake):
      current_(current),
      fake_(fake)
    {}

    BSTConstIterator(const BSTIterator< Key, Value >& other):
      current_(other.current_),
      fake_(other.fake_)
    {}

    const std::pair< Key, Value >& operator*() const
    {
      if (current_ == fake_)
      {
        throw std::runtime_error("bad iterator");
      }

      return current_->data_;
    }

    const std::pair< Key, Value >* operator->() const
    {
      return &current_->data_;
    }

    BSTConstIterator& operator++()
    {
      if (current_ != fake_)
      {
        current_ =
          nextNode(
            const_cast< NodeT* >(current_),
            const_cast< NodeT* >(fake_)
          );
      }

      return *this;
    }

    BSTConstIterator operator++(int)
    {
      BSTConstIterator temp(*this);
      ++(*this);
      return temp;
    }

    bool operator==(const BSTConstIterator& other) const noexcept
    {
      return current_ == other.current_;
    }

    bool operator!=(const BSTConstIterator& other) const noexcept
    {
      return !(*this == other);
    }

  private:
    const NodeT* current_;
    const NodeT* fake_;

    template< class K, class V >
    friend class BSTree;
  };
}

#endif
