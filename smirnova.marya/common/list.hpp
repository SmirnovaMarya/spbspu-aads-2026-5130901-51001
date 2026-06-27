#ifndef LIST_HPP
#define LIST_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <limits>
#include <stdexcept>

namespace smirnova
{
  template < class T >
  struct Node
  {
    T data;
    Node* next;
    Node* prev;

    Node():
      next(nullptr), prev(nullptr)
    {}

    Node(const T& d):
      data(d), next(nullptr), prev(nullptr)
    {}

    Node(T&& d):
      data(std::move(d)), next(nullptr), prev(nullptr)
    {}
  };

  template < class T >
  class LIter;

  template < class T >
  class LCIter;

  template < class T >
  class List
  {
  private:
    Node< T >* sentinel;
    size_t count;
    void remove(Node< T >* node)
    {
      node->prev->next = node->next;
      node->next->prev = node->prev;
    }
    void insertBefore(Node<T>* node, Node<T>* newNode)
    {
      newNode->next = node;
      newNode->prev = node->prev;

      node->prev->next = newNode;
      node->prev = newNode;
    }

  public:
    List():
      count(0)
    {
      sentinel = new Node< T >();
      sentinel->next = sentinel;
      sentinel->prev = sentinel;
    }

    void clear() noexcept
    {
      Node< T >* current = sentinel->next;
      while (current != sentinel) {
        Node< T >* tmp = current->next;
        delete current;
        current = tmp;
      }
      sentinel->next = sentinel;
      sentinel->prev = sentinel;
      count = 0;
    }

    ~List()
    {
      clear();
      delete sentinel;
    }

    List(const List& other):
      List()
      {
        for (LCIter< T > it = other.cbegin();it.valid(); it.next())
        {
          pushBack(it.value());
        }
      }

    List(List&& other) noexcept:
      sentinel(other.sentinel), count(other.count)
    {
      other.sentinel = new Node< T >(T());
      other.sentinel->next = other.sentinel;
      other.sentinel->prev = other.sentinel;
      other.count = 0;
    }

    List& operator=(const List& other)
    {
      if (this != &other) {
        List tmp(other);
        swap(tmp);
      }
      return *this;
    }

    List& operator=(List&& other) noexcept
    {
      if (this != &other) {
        clear();
        delete sentinel;

        sentinel = other.sentinel;
        count = other.count;

        other.sentinel = new Node< T >(T());
        other.sentinel->next = other.sentinel;
        other.sentinel->prev = other.sentinel;
        other.count = 0;
      }
      return *this;
    }

    void splice(LIter< T > pos, List& other, LIter<T> it)
    {
      if (!it.valid())
      {
        return;
      }
      if (this == &other && pos.node == it.node)
      {
        return;
      }
      Node< T >* node = it.node;
      other.unlink(node);
      --other.count;
      insertBefore(pos.node, node);
      ++count;
    }

    void splice(LIter< T > pos, List& other)
    {
      if (other.empty())
      {
        return;
      }
      Node< T >* first = other.sentinel->next;
      Node< T >* last = other.sentinel->prev;
      Node< T >* p = pos.node;
      first->prev = p->prev;
      p->prev->next = first;
      last->next = p;
      p->prev = last;
      count += other.count;
      other.sentinel->next = other.sentinel;
      other.sentinel->prev = other.sentinel;
      other.count = 0;
    }

    void splice(LIter<T> pos,
            List& other,
            LIter<T> first,
            LIter<T> last)
    {
      if (first.node == last.node)
      {
          return;
      }
      if (this == &other &&
          (pos.node == first.node || pos.node == last.node))
      {
          return;
      }
      size_t moved = 0;
      for (LIter<T> it = first; it.node != last.node; it.next())
      {
          ++moved;
      }
      Node<T>* firstNode = first.node;
      Node<T>* afterLast = last.node;
      Node<T>* lastNode = afterLast->prev;
      Node<T>* posNode = pos.node;

      firstNode->prev->next = afterLast;
      afterLast->prev = firstNode->prev;
      firstNode->prev = posNode->prev;
      posNode->prev->next = firstNode;
      lastNode->next = posNode;
      posNode->prev = lastNode;

      count += moved;
      other.count -= moved;
    }

    void swap(List& other) noexcept
    {
      std::swap(sentinel, other.sentinel);
      std::swap(count, other.count);
    }

    Node< T >* frontNode()
    {
      return sentinel->next;
    }

    Node< T >* backNode()
    {
      return sentinel->prev;
    }

    bool empty() const noexcept
    {
      return count == 0;
    }
    size_t size() const noexcept
    {
      return count;
    }

    T& front()
    {
      if (empty()) {
        throw std::out_of_range("empty");
      }
      return sentinel->next->data;
    }

    const T& front() const
    {
      if (empty()) {
        throw std::out_of_range("empty");
      }
      return sentinel->next->data;
    }

    T& back()
    {
      if (empty()) {
        throw std::out_of_range("empty");
      }
      return sentinel->prev->data;
    }

    const T& back() const
    {
      if (empty()) {
        throw std::out_of_range("empty");
      }
      return sentinel->prev->data;
    }

    void pushBack(const T& val)
    {
      Node< T >* n = new Node< T >(val);
      n->next = sentinel;
      n->prev = sentinel->prev;
      sentinel->prev->next = n;
      sentinel->prev = n;
      ++count;
    }

    void pushBack(T&& val)
    {
      Node< T >* n = new Node< T >(std::move(val));
      n->next = sentinel;
      n->prev = sentinel->prev;
      sentinel->prev->next = n;
      sentinel->prev = n;
      ++count;
    }

    void pushFront(const T& val)
    {
      Node< T >* n = new Node< T >(val);
      n->prev = sentinel;
      n->next = sentinel->next;
      sentinel->next->prev = n;
      sentinel->next = n;
      ++count;
    }

    void pushFront(T&& val)
    {
      Node< T >* n = new Node< T >(std::move(val));
      n->prev = sentinel;
      n->next = sentinel->next;
      sentinel->next->prev = n;
      sentinel->next = n;
      ++count;
    }

    void popFront()
    {
      if (empty()) {
        throw std::out_of_range("pop_front on empty list");
      }
      Node< T >* tmp = sentinel->next;
      sentinel->next = tmp->next;
      tmp->next->prev = sentinel;
      delete tmp;
      --count;
    }

    void popBack()
    {
      if (empty()) {
        throw std::out_of_range("pop_back on empty list");
      }
      Node< T >* tmp = sentinel->prev;
      sentinel->prev = tmp->prev;
      tmp->prev->next = sentinel;
      delete tmp;
      --count;
    }

    LIter< T > begin()
    {
      return LIter< T >(sentinel->next, sentinel);
    }
    LIter< T > end()
    {
      return LIter< T >(sentinel, sentinel);
    }
    LCIter< T > begin() const
    {
      return LCIter< T >(sentinel->next, sentinel);
    }
    LCIter< T > end() const
    {
      return LCIter< T >(sentinel, sentinel);
    }
    LCIter< T > cbegin() const
    {
      return LCIter< T >(sentinel->next, sentinel);
    }
    LCIter< T > cend() const
    {
      return LCIter< T >(sentinel, sentinel);
    }
  };

  template < class T >
  class LIter
  {
    friend class List< T >;
  private:
    Node< T >* node;
    Node< T >* sentinel;

  public:
    LIter(Node< T >* n = nullptr, Node< T >* s = nullptr):
      node(n), sentinel(s)
    {}

    bool valid() const
    {
      return node != sentinel;
    }

    void next()
    {
      if (!valid()) {
        throw std::out_of_range("iterator cannot advance past end()");
      }
      node = node->next;
    }

    T& value()
    {
      if (!valid()) {
        throw std::out_of_range("iterator is end()");
      }
      return node->data;
    }

    const T& value() const
    {
      if (!valid()) {
        throw std::out_of_range("iterator is end()");
      }
      return node->data;
    }
  };

  template < class T >
  class LCIter
  {
    friend class List< T >;
  private:
    const Node< T >* node;
    const Node< T >* sentinel;

  public:
    LCIter(const Node< T >* n = nullptr, const Node< T >* s = nullptr):
      node(n), sentinel(s)
    {}

    bool valid() const
    {
      return node != sentinel;
    }

    void next()
    {
      if (node) {
        node = node->next;
      }
    }

    const T& value() const
    {
      if (!valid()) {
        throw std::out_of_range("iterator is end()");
      }
      return node->data;
    }
  };

}

#endif

