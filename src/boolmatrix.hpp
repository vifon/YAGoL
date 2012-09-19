// File: boolmatrix.hpp
#ifndef _BOOLMATRIX_H_
#define _BOOLMATRIX_H_

#include <vector>
#include <utility>

//////////////////////////////////////////////////////////////////////

class BoolMatrix
{
    typedef std::vector<bool> data_type;

  public:
    BoolMatrix(const size_t w, const size_t h)
        : width_(w)
        , height_(h)
        , data_(w * h)
    {}

    BoolMatrix(const size_t w, const size_t h, const bool val)
        : width_(w)
        , height_(h)
        , data_(w * h, val)
    {}

    ////////////////////////////////////////

    typedef data_type::reference       reference;
    typedef data_type::const_reference const_reference;
    typedef data_type::iterator        iterator;
    typedef data_type::const_iterator  const_iterator;

    ////////////////////////////////////////

    reference operator()(int x, int y)
    {
        x = (x + width_ ) % width_;
        y = (y + height_) % height_;

        return data_.at(x + y*width_);
    }

    ////////////////////////////////////////

    const_reference operator()(int x, int y) const
    {
        x = (x + width_ ) % width_;
        y = (y + height_) % height_;

        return data_.at(x + y*width_);
    }

    ////////////////////////////////////////

    void resize(const size_t w, const size_t h)
    {
        BoolMatrix tmp(w,h, false);

        for (size_t y = 0; y < height_; ++y) {
            for (size_t x = 0; x < width_; ++x) {
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

#endif
