/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef HISTORYFIFO_H
#define HISTORYFIFO_H

#include <iostream>

#include <QVector>
#include <QDateTime>

/** @addtogroup common
 *  @{
 */

template <class T> class HistoryFifo
{
public:

  typedef T value_type;
  typedef size_t size_type;

  explicit HistoryFifo(size_type n, const value_type& value = value_type())
  {
    size_ = n;
    timestamp_.resize(size_);
    buffer_.resize(size_);
    for (size_t i=0;i<size_;++i) {
      timestamp_[i] = QDateTime::currentDateTime();
      buffer_[i] = value;
    }
  }

  void resize(size_type n, const value_type& value = value_type())
  {
    size_ = n;
    buffer_.resize(size_);
    buffer_.resize(n);
    for (size_t i=size_;i<n;++i) {
      timestamp_[i] = QDateTime::currentDateTime();
      buffer_[i] = value;
    }
    size_ = n;
  }

  size_type size()
  {
    return size_;
  }

  virtual void push_back(const value_type& value)
  {
    push_back(QDateTime::currentDateTime(), value);
  }

  virtual void push_back(const QDateTime& dt, const value_type& value)
  {
    currentIdx_++;
    if (currentIdx_>=size_) currentIdx_ = 0;
    timestamp_[currentIdx_] = dt;
    buffer_[currentIdx_] = value;
  }

  virtual QDateTime & timeAt(size_type pos)
  {
    if (pos>=size_) throw std::out_of_range("HistoryFifo index out of range.");

    size_type thePos;
    if (currentIdx_>=pos) {
      thePos = currentIdx_ - pos;
    } else {
      thePos = size_ + currentIdx_ - pos;
    }
    
    return timestamp_[thePos];
  }

  virtual const QDateTime & timeAt(size_type pos) const
  {
    if (pos>=size_) throw std::out_of_range("HistoryFifo index out of range.");

    size_type thePos;
    if (currentIdx_>=pos) {
      thePos = currentIdx_ - pos;
    } else {
      thePos = size_ + currentIdx_ - pos;
    }
    
    return timestamp_[thePos];
  }

  virtual value_type& at(size_type pos)
  {
    if (pos>=size_) throw std::out_of_range("HistoryFifo index out of range.");

    size_type thePos;
    if (currentIdx_>=pos) {
      thePos = currentIdx_ - pos;
    } else {
      thePos = size_ + currentIdx_ - pos;
    }
    
    return buffer_[thePos];
  }

  virtual const value_type& at(size_type pos) const
  {
    if (pos>=size_) throw std::out_of_range("HistoryFifo index out of range.");

    size_type thePos;
    if (currentIdx_>=pos) {
      thePos = currentIdx_ - pos;
    } else {
      thePos = size_ + currentIdx_ - pos;
    }
    
    return buffer_[thePos];
  }

  QDateTime & timeFront()
  {
    return timeAt(0);
  }

  const QDateTime & timeFront() const
  {
    return timeAt(0);
  }

  QDateTime & timeBack() {
    return timeAt(size_-1);
  }

  const QDateTime & timeBack() const
  {
    return timeAt(size_-1);
  }
  
  virtual const value_type& front() const
  {
    return at(0);
  }

  virtual const value_type& back() const
  {
    return at(size_-1);
  }

  qint64 sizeInSecs() const {
    const QDateTime & f = timeFront();
    const QDateTime & l = timeBack();
    
    return l.secsTo(f);
  }

  qint64 deltaTime() const {
    const QDateTime & f = timeFront();
    const QDateTime & l = timeBack();
    
    return f.secsTo(l);
  }
  
  qint64 deltaTime(int i, int j) const {
    const QDateTime & f = timeAt(i);
    const QDateTime & l = timeAt(j);
    
    return f.secsTo(l);
  }

  qint64 delta() const {
    const T & f = front();
    const T & l = back();
    
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

  size_type size_;
  size_type currentIdx_;

  QVector<QDateTime> timestamp_;
  QVector<T> buffer_;
};

/** @} */

#endif // HISTORYFIFO_H
