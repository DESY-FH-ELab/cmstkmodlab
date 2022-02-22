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

#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <iostream>
#include <array>
#include <stdexcept>

/** @addtogroup common
 *  @{
 */

template <class T, size_t N> class Ringbuffer
{
public:

  typedef T value_type;
  typedef size_t size_type;
  
  explicit Ringbuffer(const value_type& value = value_type())
  {
    size_ = N;
    for (size_t i=0;i<size_;++i) buffer_[i] = value;
    currentIdx_ = size_-1;
  }

  size_type size()
  {
    return N;
  }

  virtual void push_back(const value_type& value)
  {
    currentIdx_++;
    if (currentIdx_>=size_) currentIdx_ = 0;
    buffer_[currentIdx_] = value;
  }

  virtual value_type& at(size_type pos)
  {
    if (pos>=size_) throw std::out_of_range("Ringbuffer index out of range.");

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
    if (pos>=size_) throw std::out_of_range("Ringbuffer index out of range.");

    size_type thePos;
    if (currentIdx_>=pos) {
      thePos = currentIdx_ - pos;
    } else {
      thePos = size_ + currentIdx_ - pos;
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
    return at(size_-1);
  }

  class iterator
  {
  public:

    typedef size_t size_type;

    iterator(size_type position, Ringbuffer& ringbuffer)
      : position_(position), ringbuffer_(ringbuffer) { }

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
      return ringbuffer_.at(position_);
    }

    bool operator!=(const iterator& other) const
    {
      return (position_ != other.position_);
    }
  
  private:
  
    size_type position_;
    Ringbuffer<T,N>& ringbuffer_;
  };
  
  iterator begin()
  {
    return iterator(0, *this);
  }

  iterator end()
  {
    return iterator(size_, *this);
  }

  class const_iterator
  {
  public:

    typedef size_t size_type;

    const_iterator(size_type position, const Ringbuffer& ringbuffer)
      : position_(position), ringbuffer_(ringbuffer) { }

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
      return ringbuffer_.at(position_);
    }

    bool operator!=(const iterator& other) const
    {
      return (position_ != other.position_);
    }
  
  private:
  
    size_type position_;
    const Ringbuffer<T,N>& ringbuffer_;
  };
  
  const const_iterator cbegin() const
  {
    return const_iterator(0, *this);
  }

  const const_iterator cend() const
  {
    return const_iterator(size_, *this);
  }
    
 protected:

  size_type size_;
  size_type currentIdx_;
  std::array<T,N> buffer_;
};

/** @} */

#endif // RINGBUFFER_H
