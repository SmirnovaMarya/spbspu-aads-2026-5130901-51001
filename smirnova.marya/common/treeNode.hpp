#ifndef TREENODE_HPP
#define TREENODE_HPP

#include <utility>

namespace smirnova
{
  struct TreeNodeBase
  {
    TreeNodeBase* parent_;
    TreeNodeBase* left_;
    TreeNodeBase* right_;

    TreeNodeBase():
      parent_(nullptr),
      left_(nullptr),
      right_(nullptr)
    {}
  };

  template< class Key, class Value >
  struct TreeNode:
    TreeNodeBase
  {
    std::pair<Key, Value> data_;

    TreeNode(const std::pair<Key, Value>& data):
      data_(data)
    {}
  };
}

#endif

