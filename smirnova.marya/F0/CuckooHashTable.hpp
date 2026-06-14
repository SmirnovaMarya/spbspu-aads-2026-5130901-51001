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

    class Iterator
    {
    public:
      using Table = CuckooHashTable;

      Iterator(Table* t = nullptr, std::size_t i = 0):
        table_(t),
        index_(i)
      {
        skip();
      }

      Node& operator*()
      {
        auto p = resolve(index_);
        return table_->bucket(p.first)[p.second];
      }

      Node* operator->()
      {
        return &(**this);
      }

      Iterator& operator++()
      {
        ++index_;
        skip();
        return *this;
      }

      bool operator!=(const Iterator& other) const
      {
        return table_ != other.table_ || index_ != other.index_;
      }
    private:
      void skip()
      {
        if (!table_)
        {
          return;
        }
        std::size_t total = table_->capacity() * 2;
        while (index_ < total)
        {
          auto p = resolve(index_);
          auto& b = table_->bucket(p.first);
          if (b[p.second].used && !b[p.second].deleted)
          {
            return;
          }
          ++index_;
        }
      }
    }

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
    double loadFactor() const
    {
      std::size_t used = 0;

      for (std::size_t i = 0; i < capacity_; ++i)
      {
        if (table1_[i].used)
        {
          ++used;
        }
        if (table2_[i].used)
        {
          ++used;
        }
      }
      return (double)used / (capacity_ * 2);
    }
    bool erase(const Key& key)
    {
      std::size_t i1 = hash_.h1(key) % capacity_;

      if (table1_[i1].used && table1_[i1].key == key)
      {
        table1_[i1].deleted = true;
        return true;
      }

      std::size_t i2 = hash_.h2(key) % capacity_;

      if (table2_[i2].used && table2_[i2].key == key)
      {
        table2_[i2].deleted = true;
        return true;
      }
      return false;
    }
    bool insert(const Key& key, const Value& value)
    {
      if (loadFactor() > 0.7)
      {
        rehash(capacity_ * 2);
      }

      std::size_t i1 = hash_.h1(key) % capacity_;

      if (!table1_[i1].used)
      {
        table1_[i1] = {key, value, true, false};
        return true;
      }

      std::size_t i2 = hash_.h2(key) % capacity_;

      if (!table2_[i2].used)
      {
        table2_[i2] = {key, value, true, false};
        return true;
      }
      return false;
    }
    Iterator begin()
    {
      return Iterator(this, 0);
    }

    Iterator end()
    {
      return Iterator(this, capacity_ * 2);
    }
  private:
    void allocate(size_t n)
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

