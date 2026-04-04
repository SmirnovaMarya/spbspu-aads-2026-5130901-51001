#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "list.hpp"
#include <stdexcept>

namespace smirnova {

template <typename T>
class Queue {
private:
    List<T> data;

public:
    void push(const T& value) {
        data.pushBack(value);
    }

    void pushBack(const T& value) {
        data.pushBack(value);
    }

    T drop() {
        if (data.empty()) {
            throw std::logic_error("queue is empty");
        }
        T val = data.frontNode()->data;
        data.popFront();
        return val;
    }

    T popFront() {
        return drop();
    }

    T& front() {
        if (data.empty()) {
            throw std::logic_error("queue is empty");
        }
        return data.frontNode()->data;
    }

    const T& front() const {
        if (data.empty()) {
            throw std::logic_error("queue is empty");
        }
        return data.frontNode()->data;
    }

    bool empty() const {
        return data.empty();
    }

    size_t size() const {
        return data.size();
    }
};

}

#endif