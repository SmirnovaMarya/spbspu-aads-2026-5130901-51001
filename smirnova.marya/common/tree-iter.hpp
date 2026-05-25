#ifndef TREEITERATORS_HPP
#define TREEITERATORS_HPP

#include <stdexcept>
#include <utility>

// `TreeNodeBase` defined in `bstree.hpp`; included by TUs that include this file

namespace smirnova
{
  

  TreeNodeBase* nextNodeBase(TreeNodeBase* node, TreeNodeBase* fake)
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

    TreeNodeBase* parent = node->parent_;

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

    BSTIterator(TreeNodeBase* current, TreeNodeBase* fake):
      current_(current),
      fake_(fake)
    {}

    std::pair< Key, Value >& operator*() const
    {
      if (current_ == fake_)
      {
        throw std::runtime_error("bad iterator");
      }

      return static_cast< NodeT* >(current_)->data_;
    }

    std::pair< Key, Value >* operator->() const
    {
      return &static_cast< NodeT* >(current_)->data_;
    }

    BSTIterator& operator++()
    {
      if (current_ != fake_)
      {
        current_ = nextNodeBase(current_, fake_);
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
    TreeNodeBase* current_;
    TreeNodeBase* fake_;

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

    BSTConstIterator(const TreeNodeBase* current, const TreeNodeBase* fake):
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

      return static_cast< const NodeT* >(current_)->data_;
    }

    const std::pair< Key, Value >* operator->() const
    {
      return &static_cast< const NodeT* >(current_)->data_;
    }

    BSTConstIterator& operator++()
    {
      if (current_ != fake_)
      {
        current_ = nextNodeBase(const_cast< TreeNodeBase* >(current_), const_cast< TreeNodeBase* >(fake_));
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
    const TreeNodeBase* current_;
    const TreeNodeBase* fake_;

    template< class K, class V >
    friend class BSTree;
  };
}

#endif

