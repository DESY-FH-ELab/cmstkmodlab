#ifndef TKMLVECTOR_h
#define TKMLVECTOR_h

#include <iostream>
#include <vector>

namespace TkML {

  template<typename T>
  class Vector
  {
  public:

    Vector() : data_(0) { }

    explicit Vector(size_t n) : data_(n) { }

    Vector(size_t n, const T& v) : data_(n, v) { }

    Vector(size_t n, const T* v) : data_(n) {
      for (size_t i=0;i<n;++i) {
        data_[i] = v[i];
      }
    }

    template<typename U>
    Vector(const TkML::Vector<U>& other) : data_(other.size()) {
      for (size_t i=0;i<size();++i) {
        data_[i] = other[i];
      }
    }

    ~Vector() { }

    Vector & operator=(const TkML::Vector<T>& other) {
      if (this!=&other) {
        data_.resize(other.size());
        for (size_t i=0;i<size();++i) {
          data_[i] = other[i];
        }
      }
      return *this;
    }

    template<typename U>
    Vector & operator=(const TkML::Vector<U>& other) {
      data_.resize(other.size());
      for (size_t i=0;i<size();++i) {
        data_[i] = other[i];
      }
      return *this;
    }

    T at(const size_t i) {
      return data_[i];
    }

    const T at(const size_t i) const {
      return data_[i];
    }

    T & operator[](const size_t i) {
      return data_[i];
    }

    const T & operator[](const size_t i) const {
      return data_[i];
    }

    size_t size() const { return data_.size(); }

    void print() const {
      for (size_t i=0;i<size();++i) {
        std::cout << this->operator[](i) << " ";
      }
      std::cout << std::endl;
    }

  protected:

    std::vector<T> data_;
  };
};

#endif // TKMLVECTOR_h
