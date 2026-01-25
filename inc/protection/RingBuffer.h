#pragma once

#include <iostream>
#include <vector>
#include <optional>


// wraps a vector of std::optional<T> where T is the template parameter.
// has two main methods: tryPush, tryPop.


template <typename T>
class RingBuffer
{
private:    
    std::vector<std::optional<T>> _buffer;
    std::size_t _head;
    std::size_t _tail;
    bool _full;

    void advance(std::size_t& index) {
        index = (index + 1) % _buffer.size();
    }
public:
    explicit RingBuffer(std::size_t capacity)
        : _buffer(capacity), _head(0), _tail(0), _full(false) {}

    // supports move semantics.
    RingBuffer(RingBuffer&& other) noexcept = default;
    RingBuffer& operator=(RingBuffer&& other) noexcept = default;
    // does not support copy semantics.
    RingBuffer(const RingBuffer&) = delete;
    RingBuffer& operator=(const RingBuffer&) = delete;

    bool tryPush(T&& value) {
        if (_full) return false;
        _buffer[_head] = std::move(value);
        advance(_head);
        if (_head == _tail) _full = true;
        return true;
    }

    std::optional<T> tryPop() {
        if (empty()) return std::nullopt;
        std::optional<T> value = std::move(_buffer[_tail]);
        _buffer[_tail].reset();
        _full = false;
        advance(_tail);
        return value;
    }

    bool empty() const {
        return (!_full && _head == _tail);
    }

    bool full() const {
        return _full;
    }

    std::size_t capacity() const {
        return _buffer.size();
    }

    std::size_t size() const {
        if (_full) return _buffer.size();
        if (_head >= _tail) return _head - _tail;
        return _buffer.size() - _tail + _head;
    }

};
