/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef FIFO_H
#define FIFO_H

#include <iostream>

#include <QVector>

/** @addtogroup common
 *  @{
 */

template <class T> class Fifo
{
public:

  typedef T value_type;
  typedef size_t size_type;
  
  explicit Fifo(size_type n = 1, const value_type& value = value_type())
  {
    size_ = n;
    buffer_.resize(size_);
    for (size_t i=0;i<size_;++i) buffer_[i] = value;
    currentIdx_ = size_-1;
    fillLevel_ = 0;
  }

  void resize(size_type n, const value_type& value = value_type())
  {
    size_ = n;
    buffer_.resize(n);
    for (size_t i=size_;i<n;++i) buffer_[i] = value;
    size_ = n;
  }

  size_type size()
  {
    return size_;
  }

  virtual void push_back(const value_type& value)
  {
    currentIdx_++;
    if (currentIdx_>=size_) currentIdx_ = 0;
    buffer_[currentIdx_] = value;
    if (fillLevel_<size_) fillLevel_++;
  }

  virtual value_type& at(size_type pos)
  {
    if (pos>=size_) throw std::out_of_range("Fifo index out of range.");

    size_type thePos = pos;
    if (thePos>=fillLevel_) thePos = fillLevel_-1;
    if (currentIdx_>=thePos) {
      thePos = currentIdx_ - thePos;
    } else {
      thePos = size_ + currentIdx_ - thePos;
    }
    
    return buffer_[thePos];
  }

  virtual const value_type& at(size_type pos) const
  {
    if (pos>=size_) throw std::out_of_range("Fifo index out of range.");

    size_type thePos = pos;
    if (thePos>=fillLevel_) thePos = fillLevel_-1;
    if (currentIdx_>=thePos) {
      thePos = currentIdx_ - thePos;
    } else {
      thePos = size_ + currentIdx_ - thePos;
    }
    
    return buffer_[thePos];
  }

  virtual value_type& operator[](size_type pos)
  {
    return at(pos);
  }

  virtual const value_type& operator[](size_type pos) const
  {
    return at(pos);
  }

  virtual const value_type& front() const
  {
    return at(0);
  }

  virtual const value_type& back() const
  {
    return at(fillLevel_-1);
  }

  class iterator
  {
  public:

    typedef size_t size_type;

    iterator(size_type position, Fifo& fifo)
      : position_(position), fifo_(fifo) { }

    iterator& operator++(int)
    {
      position_++;
      return *this;
    }

    iterator& operator++()
    {
      ++position_;
      return *this;
    }

    T& operator*()
    {
      return fifo_.at(position_);
    }

    bool operator!=(const iterator& other) const
    {
      return (position_ != other.position_);
    }
  
  private:
  
    size_type position_;
    Fifo<T>& fifo_;
  };
  
  iterator begin()
  {
    return iterator(0, *this);
  }

  iterator end()
  {
    return iterator(fillLevel_, *this);
  }

  class const_iterator
  {
  public:

    typedef size_t size_type;

    const_iterator(size_type position, const Fifo& fifo)
      : position_(position), fifo_(fifo) { }

    const_iterator& operator++(int)
    {
      position_++;
      return *this;
    }

    const_iterator& operator++()
    {
      ++position_;
      return *this;
    }

    const T& operator*() const
    {
      return fifo_.at(position_);
    }

    bool operator!=(const iterator& other) const
    {
      return (position_ != other.position_);
    }
  
  private:
  
    size_type position_;
    const Fifo<T>& fifo_;
  };
  
  const const_iterator cbegin() const
  {
    return const_iterator(0, *this);
  }

  const const_iterator cend() const
  {
    return const_iterator(fillLevel_, *this);
  }
    
 protected:

  size_type size_;
  size_type currentIdx_;
  size_type fillLevel_;
  QVector<T> buffer_;
};

/** @} */

#endif // FIFO_H
