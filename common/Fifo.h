#ifndef FIFO_H
#define FIFO_H

#include <iostream>

#include <QVector>

template <class T,size_t N=5> class Fifo
{
public:

    typedef T      value_type;

    Fifo() {
    	buffer_.resize(N);
    	size_ = 0;
    	capacity_ = N;
    	currentFirstIndex_ = 0;
    }

    int size() const {
    	return size_;
    }

    virtual void push(const T& value) {
    	if (size_<capacity_) {
    		buffer_[size_] = value;
    		size_++;
    		currentFirstIndex_ = size_ - 1;
    	} else {
    		currentFirstIndex_++;
    		if (currentFirstIndex_>=capacity_) currentFirstIndex_ = 0;
    		buffer_[currentFirstIndex_] = value;
    	}
    }

    const T & at(int i) const {
    	if (i>=size_) return last();
    	int idx = currentFirstIndex_ - i;
    	if (idx<0) idx += size_;
    	return buffer_[idx];
    }

    T & first() {
    	return buffer_[currentFirstIndex_];
    }

    const T & first() const {
    	return buffer_[currentFirstIndex_];
    }

    T & last() {
    	int idx = currentFirstIndex_;
    	if (size_<capacity_) {
    		idx = 0;
    	} else {
    		idx++;
    		if (idx>=capacity_) idx = 0;
    	}
    	return buffer_[idx];
    }

    const T & last() const {
    	int idx = currentFirstIndex_;
    	if (size_<capacity_) {
    		idx = 0;
    	} else {
    		idx++;
    		if (idx>=capacity_) idx = 0;
    	}
    	return buffer_[idx];
    }

protected:

    int size_;
    int capacity_;
    int currentFirstIndex_;

    QVector<T> buffer_;
};

#endif // RINGBUFFER_H
