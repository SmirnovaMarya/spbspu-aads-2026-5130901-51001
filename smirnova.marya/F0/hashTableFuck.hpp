#pragma once

#include <cstddef>
#include <functional>

namespace smirnova
{
  template<class Key>
  struct DefaultHasher
  {
    std::size_t h1(const Key& key) const noexcept
    {
      return std::hash<Key>{}(key);
    }

    std::size_t h2(const Key& key) const noexcept
    {
      std::size_t h = std::hash<Key>{}(key);
      return std::hash<std::size_t>{}(h);
    }
  };
}

