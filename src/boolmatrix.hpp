// File: boolmatrix.hpp
#ifndef _BOOLMATRIX_H_
#define _BOOLMATRIX_H_

#include <vector>
#include <utility>

//////////////////////////////////////////////////////////////////////

class BoolMatrix : public std::vector<bool>
{
  public:
    BoolMatrix(const size_t w, const size_t h)
        : std::vector<bool>(w * h)
        , width_(w)
        , height_(h)
    {}

    BoolMatrix(const size_t w, const size_t h, const bool val)
        : std::vector<bool>(w * h, val)
        , width_(w)
        , height_(h)
    {}


    ////////////////////////////////////////

    std::vector<bool>::reference operator()(int x, int y)
    {
        x = x % width_;
        y = y % height_;

        return at(x + y*width_);
    }

    ////////////////////////////////////////

    std::vector<bool>::const_reference operator()(int x, int y) const
    {
        x = x % width_;
        y = y % height_;

        return at(x + y*width_);
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

  private:
    size_t width_;
    size_t height_;
};

//////////////////////////////////////////////////////////////////////

#endif
