// File: lexical_cast.hpp
#ifndef _VFN_LEXICAL_CAST_H_
#define _VFN_LEXICAL_CAST_H_

#include <sstream>
#include <typeinfo>

namespace vfn {
    template <typename T1, typename T2>
    T1 lexical_cast(T2 source) throw(std::bad_cast) {
        T1 result;
        std::stringstream stream;
        stream << source;
        stream >> result;
        if (stream.fail() || !stream.eof()) {
            throw std::bad_cast();
        }
        return result;
    }
}

#endif
