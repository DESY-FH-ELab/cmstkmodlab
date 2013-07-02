#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <iostream>

template <class T> class Ringbuffer
{
public:

    typedef T      value_type;

    explicit Ringbuffer(size_t n, const T& value = value_type()) {
        size_ = n;
        buffer_ = new T[n];
        for (size_t i=0;i<n;++i) buffer_[i] = value;
        currentWritePointer_ = 0;
        currentReadPointer_ = size_ - 1;
    }

    virtual void push_back(const T& value) {
        buffer_[currentWritePointer_] = value;
        currentWritePointer_++;
        if (currentWritePointer_>=size_) currentWritePointer_ = 0;
        currentReadPointer_ = currentWritePointer_;
    }

    virtual const T& get() {
        return buffer_[currentReadPointer_];
    }

protected:

    size_t size_;
    size_t currentWritePointer_;
    size_t currentReadPointer_;
    T* buffer_;
};

#endif // RINGBUFFER_H
