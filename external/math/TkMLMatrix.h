#ifndef TKMLMATRIX_h
#define TKMLMATRIX_h

#include <iostream>
#include <vector>

#include <TkMLTools.h>
#include <TkMLVector.h>

namespace TkML {

  template<typename T>
  class Matrix
  {
    class MatrixRow
    {
    public:

      MatrixRow() : data_(0) { }

      explicit MatrixRow(size_t n) : data_(n) { }

      MatrixRow(size_t n, const T& v) : data_(n, v) { }

      size_t size() const { return data_.size(); }

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

      void print() const {
        for (size_t i=0;i<size();++i) {
          std::cout << this->operator[](i) << " ";
        }
        std::cout << std::endl;
      }

    protected:

      std::vector<T> data_;
    };

  public:

    Matrix()
    : rows_(0),
      columns_(0) { }

    explicit Matrix(size_t n, size_t m)
    : rows_(n, MatrixRow(m)),
      columns_(m) { }

    Matrix(size_t n, size_t m, const T& v)
    : rows_(n, MatrixRow(m, v)),
      columns_(m) { }

    Matrix(size_t n, size_t m, const T* v)
    : rows_(n, MatrixRow(m)),
      columns_(m) {
      for (size_t i=0;i<n;++i) {
        for (size_t j=0;j<m;++j) {
          rows_[i][j] = *v++;
        }
      }
    }

    template<typename U>
    Matrix(const TkML::Matrix<U>& other)
    : rows_(other.rows(), MatrixRow(other.columns())),
      columns_(other.columns()) {
      for (size_t i=0;i<other.rows();++i) {
        for (size_t j=0;j<other.columns();++j) {
          rows_[i][j] = other[i][j];
        }
      }
    }

    ~Matrix() { }

    Matrix & operator=(const TkML::Matrix<T>& other) {
      if (this!=&other) {
        rows_.resize(other.rows(), MatrixRow(other.columns()));
        columns_ = other.columns();
        for (size_t i=0;i<other.rows();++i) {
          for (size_t j=0;j<other.columns();++j) {
            rows_[i][j] = other[i][j];
          }
        }
      }
      return *this;
    }

    template<typename U>
    Matrix & operator=(const TkML::Matrix<U>& other) {
      rows_.resize(other.rows(), MatrixRow(other.columns()));
      columns_ = other.columns();
      for (size_t i=0;i<other.rows();++i) {
        for (size_t j=0;j<other.columns();++j) {
          rows_[i][j] = other[i][j];
        }
      }
      return *this;
    }

    MatrixRow & operator[](const size_t i) {
      return rows_[i];
    }

    const MatrixRow & operator[](const size_t i) const {
      return rows_[i];
    }

    size_t rows() const { return rows_.size(); }
    size_t columns() const { return columns_; }

    void print() const {
      for (size_t i=0;i<rows();++i) {
        this->operator[](i).print();
      }
    }

  protected:

    std::vector<MatrixRow> rows_;
    size_t columns_;
  };

};

#endif // TKMLMATRIX_h
