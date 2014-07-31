#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <iostream>

template <class T,size_t N=5> class Ringbuffer
{
public:

    typedef T      value_type;

    explicit Ringbuffer(const T& value = value_type()) {
        size_ = N;
        buffer_ = new T[size_];
        for (size_t i=0;i<size_;++i) buffer_[i] = value;
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
