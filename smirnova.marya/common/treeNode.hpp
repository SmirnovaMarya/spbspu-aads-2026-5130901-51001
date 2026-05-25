#ifndef TREENODE_HPP
#define TREENODE_HPP

#include <utility>

namespace smirnova
{
  template< class Key, class Value >
  struct Node
  {
    std::pair< Key, Value > data_;
    Node* parent_;
    Node* left_;
    Node* right_;

    Node(const Key& key, const Value& value, Node* parent = nullptr):
      data_(key, value),
      parent_(parent),
      left_(nullptr),
      right_(nullptr)
    {}
  };

}

#endif