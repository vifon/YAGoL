// File: matrix.hpp
#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <vector>
#include <stdexcept>

//////////////////////////////////////////////////////////////////////

template <typename T>
class Matrix
{
    typedef std::vector<T> data_type;

  public:
    Matrix(const size_t w, const size_t h)
        : width_(w)
        , height_(h)
        , data_(w * h)
    {}

    Matrix(const size_t w, const size_t h, const T val)
        : width_(w)
        , height_(h)
        , data_(w * h, val)
    {}

    ////////////////////////////////////////

    typedef typename data_type::reference       reference;
    typedef typename data_type::const_reference const_reference;
    typedef typename data_type::iterator        iterator;
    typedef typename data_type::const_iterator  const_iterator;

    ////////////////////////////////////////

    reference operator()(size_t x, size_t y) throw(std::out_of_range)
    {
        if (x >= width_ || y >= height_) {
            throw std::out_of_range("Matrix::operator()");
        }

        return data_.at(x + y*width_);
    }

    const_reference operator()(size_t x, size_t y) const throw(std::out_of_range)
    {
        if (x >= width_ || y >= height_) {
            throw std::out_of_range("Matrix::operator() const");
        }

        return data_.at(x + y*width_);
    }

    ////////////////////////////////////////

    void resize(const size_t w, const size_t h)
    {
        Matrix tmp(w,h, false);

        size_t common_width  = (width()  > w ? w : width());
        size_t common_height = (height() > h ? h : height());

        for (size_t y = 0; y < common_height; ++y) {
            for (size_t x = 0; x < common_width; ++x) {
                tmp(x,y) = (*this)(x,y);
            }
        }

        *this = tmp;
    }

    ////////////////////////////////////////

    size_t width() const
    {
        return width_;
    }
    size_t height() const
    {
        return height_;
    }

    ////////////////////////////////////////

    iterator begin()
    {
        return data_.begin();
    }
    const_iterator begin() const
    {
        return data_.begin();
    }

    iterator end()
    {
        return data_.end();
    }
    const_iterator end() const
    {
        return data_.end();
    }

    ////////////////////////////////////////

  private:
    size_t width_;
    size_t height_;

    data_type data_;
};

//////////////////////////////////////////////////////////////////////

template <typename T>
class WrappedMatrix : public Matrix<T>
{
  public:
    WrappedMatrix(const size_t w, const size_t h)
        : Matrix<T>(w,h)
    {}

    WrappedMatrix(const size_t w, const size_t h, const T val)
        : Matrix<T>(w,h, val)
    {}

    ////////////////////////////////////////

    typedef typename Matrix<T>::reference       reference;
    typedef typename Matrix<T>::const_reference const_reference;
    typedef typename Matrix<T>::iterator        iterator;
    typedef typename Matrix<T>::const_iterator  const_iterator;

    ////////////////////////////////////////

    reference operator()(long long x, long long y)
    {
        x = wrap(x, this->width());
        y = wrap(y, this->height());

        return Matrix<T>::operator()(x,y);
    }

    const_reference operator()(long long x, long long y) const
    {
        x = wrap(x, this->width());
        y = wrap(y, this->height());

        return Matrix<T>::operator()(x,y);
    }

    ////////////////////////////////////////

  private:
    static long long wrap(long long n, const size_t max)
    {
        while (n < 0) {
            n += max;
        }
        n %= max;

        return n;
    }
};

//////////////////////////////////////////////////////////////////////

#endif
