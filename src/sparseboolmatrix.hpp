// File: sparseboolmatrix.hpp
#ifndef _SPARSEBOOLMATRIX_H_
#define _SPARSEBOOLMATRIX_H_

#include <map>

//////////////////////////////////////////////////////////////////////

class SparseBoolMatrix : public std::map< std::pair <size_t,size_t>, bool >
{
  public:
    bool operator()(const size_t x, const size_t y)
    {
        if (count(std::make_pair(x,y)) != 0) {
            return (*this)[std::make_pair(x,y)];
        } else {
            return false;
        }
    }

    void set(const size_t x, const size_t y, const bool state)
    {
        (*this)[std::make_pair(x,y)] = state;
    }
};

//////////////////////////////////////////////////////////////////////

#endif
