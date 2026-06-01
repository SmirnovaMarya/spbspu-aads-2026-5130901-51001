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

  template< typename Key, typename Value >
  struct TreeNode:
    TreeNodeBase
  {
    std::pair< Key, Value > data_;

    // default — нужен для fake leaf
    TreeNode():
      TreeNodeBase(),
      data_()
    {}

    // основной конструктор
    TreeNode(const Key& key, const Value& value, TreeNodeBase* parent):
      TreeNodeBase(),
      data_(key, value)
    {
      parent_ = parent;
      left_ = nullptr;
      right_ = nullptr;
    }
  };
}

#endif

