#ifndef HISTORYFIFO_H
#define HISTORYFIFO_H

#include <iostream>

#include <QVector>
#include <QDateTime>

template <class T> class HistoryFifo
{
public:

    typedef T      value_type;

    HistoryFifo(int N) {
    	timestamp_.resize(N);
    	buffer_.resize(N);
    	size_ = 0;
    	capacity_ = N;
    	currentFirstIndex_ = 0;
    }

    int size() const {
    	return size_;
    }

    virtual void push(const QDateTime& dt, const T& value) {
    	if (size_<capacity_) {
    		timestamp_[size_] = dt;
     		buffer_[size_] = value;
    		size_++;
    		currentFirstIndex_ = size_ - 1;
    	} else {
    		currentFirstIndex_++;
    		if (currentFirstIndex_>=capacity_) currentFirstIndex_ = 0;
    		timestamp_[currentFirstIndex_] = dt;
    		buffer_[currentFirstIndex_] = value;
    	}
    }

    const QDateTime & timeAt(int i) const {
    	if (i>=size_) return lastTime();
    	int idx = currentFirstIndex_ - i;
    	if (idx<0) idx += size_;
    	return timestamp_[idx];
    }

    QDateTime & firstTime() {
    	return timestamp_[currentFirstIndex_];
    }

    const QDateTime & firstTime() const {
    	return timestamp_[currentFirstIndex_];
    }

    QDateTime & lastTime() {
    	int idx = currentFirstIndex_;
    	if (size_<capacity_) {
    		idx = 0;
    	} else {
    		idx++;
    		if (idx>=capacity_) idx = 0;
    	}
    	return timestamp_[idx];
    }

    const QDateTime & lastTime() const {
    	int idx = currentFirstIndex_;
    	if (size_<capacity_) {
    		idx = 0;
    	} else {
    		idx++;
    		if (idx>=capacity_) idx = 0;
    	}
    	return timestamp_[idx];
    }

    qint64 sizeInSecs() const {
    	const QDateTime & f = firstTime();
    	const QDateTime & l = lastTime();

    	return l.secsTo(f);
    }

    qint64 deltaTime() const {
    	const QDateTime & f = firstTime();
    	const QDateTime & l = lastTime();

    	return f.secsTo(l);
    }

    qint64 deltaTime(int i, int j) const {
    	const QDateTime & f = timeAt(i);
    	const QDateTime & l = timeAt(j);

    	return f.secsTo(l);
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

    qint64 delta() const {
    	const T & f = first();
    	const T & l = last();

    	return l - f;
    }

    qint64 delta(int i, int j) const {
    	const T & f = at(i);
    	const T & l = at(j);

    	return l - f;
    }

    const T gradient() const {
    	T d = delta();
    	double dt = deltaTime();

    	return d/dt;
    }

    const T gradient(int i, int j) const {
    	T d = delta(i, j);
    	double dt = deltaTime(i, j);

    	return d/dt;
    }

protected:

    int size_;
    int capacity_;
    int currentFirstIndex_;

    QVector<QDateTime> timestamp_;
    QVector<T> buffer_;
};

#endif // RINGBUFFER_H
