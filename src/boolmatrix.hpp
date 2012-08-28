// File: boolmatrix.hpp
#ifndef _BOOLMATRIX_H_
#define _BOOLMATRIX_H_

#include <vector>

//////////////////////////////////////////////////////////////////////

class BoolMatrix : public std::vector<bool>
{
  public:
    BoolMatrix(const size_t w, const size_t h)
        : std::vector<bool>(w * h)
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

  private:
    size_t width_;
    size_t height_;
};

//////////////////////////////////////////////////////////////////////

#endif
