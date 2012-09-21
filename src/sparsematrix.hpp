// File: sparsematrix.hpp
#ifndef _SPARSEMATRIX_H_
#define _SPARSEMATRIX_H_

#include <map>
#include <stdexcept>

//////////////////////////////////////////////////////////////////////

template <typename T>
class SparseMatrix
{
    typedef std::map<
                std::pair<
                    size_t,
                    size_t>,
                T>
            data_type;

  public:
    typedef typename data_type::iterator       iterator;
    typedef typename data_type::const_iterator const_iterator;

    ////////////////////////////////////////

    T operator()(const size_t x, const size_t y) const throw(std::out_of_range)
    {
        if (data_.count(std::make_pair(x,y)) == 0) {
            throw std::out_of_range("SparseMatrix::operator() const");
        }

        return data_[std::make_pair(x,y)];
    }

    ////////////////////////////////////////

    void set(const size_t x, const size_t y, const T state)
    {
        data_[std::make_pair(x,y)] = state;
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

    void clear()
    {
        return data_.clear();
    }

    ////////////////////////////////////////

  private:
    data_type data_;
};

//////////////////////////////////////////////////////////////////////

#endif
