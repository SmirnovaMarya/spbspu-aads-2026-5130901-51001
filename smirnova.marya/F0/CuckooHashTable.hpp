#pragma once

#include <cstddef>
#include "../common/vector.hpp"
#include "CuckooHashFuck.hpp"

namespace smirnova
{

  template< class Key, class Value, class Hash = DefaultHasher< Key > >
  class CuckooHashTable
  {
  public:
    struct Node
    {
      Key key{};
      Value value{};
      bool used = false;
      bool deleted = false;
    };
  private:
    Vector< Node > table1_;
    Vector< Node > table2_;
    std::size_t capacity_ = 16;
    Hash hash_;
  public:
    CuckooHashTable()
    {
      allocate(capacity_);
    }
    size_t capacity() const
    {
      return capacity_;
    }
    Vector< Node >& bucket(int i)
    {
      if (i == 0)
      {
        return table1_;
      }
      else
      {
        return table2_;
      }
    }
    const Value* get(const Key& key) const
    {
      return const_cast< CuckooHashTable* >(this)->get(key);
    }
    bool contains(const Key& key) const
    {
      return get(key) != nullptr;
    }
  private:
    void allocate(std::size_t n)
    {
      table1_.clear();
      table2_.clear();
      for (std::size_t i = 0; i < n; ++i)
      {
        table1_.pushBack(Node{});
        table2_.pushBack(Node{});
      }
  };
}

